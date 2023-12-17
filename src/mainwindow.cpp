#include <QSplitter>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "fontWidget.h"
#include "glyphsWidget.h"
#include "editWidget.h"

#include "qdir.h"
#include "settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define NEW_NAME "Новый"

static const QString programName(QStringLiteral("Font creator"));
static const QString version(QStringLiteral("0.0.1 "));


//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _projName(NEW_NAME)
{
    _ui->setupUi(this);
    _wgtFont   = new FontWidget(this);
    _ui->dockFont->setWidget(_wgtFont);
    _wgtGlyphs = new GlyphsWidget(this);
    _ui->dockGlyphs->setWidget(_wgtGlyphs);
    _wgtEdit   = new EditWidget(this);
    setCentralWidget(_wgtEdit);

    _settings = new Settings(this);

    connect(_ui->actOpenProj, qOverload<bool>(&QAction::triggered), this, &MainWindow::openProject);
    connect(_ui->actSaveProj, qOverload<bool>(&QAction::triggered), this, &MainWindow::saveProject);

    connect(_ui->actSaveCode, qOverload<bool>(&QAction::triggered), this, &MainWindow::generateCode);
    connect(_ui->actSaveCodeAs, qOverload<bool>(&QAction::triggered), this, &MainWindow::generateCodeAs);
    connect(_ui->actOpenFont, qOverload<bool>(&QAction::triggered), _wgtFont, &FontWidget::openFontClick);

    connect(_ui->actSettings, qOverload<bool>(&QAction::triggered), this, &MainWindow::settingsShow);
    connect(_ui->actAbout, qOverload<bool>(&QAction::triggered), this, &MainWindow::aboutShow);

    connect(_wgtFont, qOverload<const GlyphsMap &, const QString &>(&FontWidget::exportGlyphs),
            _wgtGlyphs, &GlyphsWidget::receiveGlyphs);

    connect(_wgtGlyphs, qOverload<const Glyph &>(&GlyphsWidget::glyphSelected),
            _wgtEdit, &EditWidget::receiveGlyph);

    connect(_wgtEdit, qOverload<const Glyph &>(&EditWidget::editFinished),
            _wgtGlyphs, &GlyphsWidget::editFinish);

    _generator.setGlyphs(_wgtGlyphs->glyphs());
    _generator.setSettings(_settings);

    _baseDir.setPath( _settings->basePath());

    setWindowTitle(NEW_NAME);
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete _ui;
}

//----------------------------------------------------------------------------------------------------------------------
Settings *MainWindow::settings() const
{
    return _settings;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------- P R I V A T E   S L O T S --------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::openProject()
{
    qDebug() << "Load project";
    QString dirStr = QFileDialog::getExistingDirectory(this, tr("Открыть проект из папки"),
                                                     _baseDir.absolutePath(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    qDebug() << "Dir: " << dirStr;
    if(dirStr.length() == 0)
    {
        return;
    }
    _projectPath.setPath(dirStr);
    load();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::saveProject()
{
    if(NEW_NAME == _projName)
    {
        QString dirStr = QFileDialog::getExistingDirectory(this, tr("Сохранить проект в папку"),
                                                         _baseDir.absolutePath(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
        if(dirStr.length() == 0)
        {
            return;
        }
        _projectPath.setPath(dirStr);
        _projName = _projectPath.dirName();
    }
    setWindowTitle(_projName);
    qDebug() << "Save path: " << _projectPath.absolutePath();
    // Очистим путь
    _projectPath.setNameFilters(QStringList() << "*");
    _projectPath.setFilter(QDir::Files);
    foreach(QString dirFile, _projectPath.entryList())
    {
        _projectPath.remove(dirFile);
    }

    save();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::generateCode()
{
    qDebug() << "Base dir: " << _baseDir.path();
    if(_wgtGlyphs->glyphs()->isEmpty())
    {
        qDebug() << "Haven`t glyphs for save code";
        return;
    }
    QDir exportDir = _baseDir;
    exportDir.mkdir(_projName + "_gen");
    if(exportDir.cd(_projName + "_gen"))
    {
        _generator.saveFont(exportDir, _wgtGlyphs->fontName());
    }
    else
    {
        qWarning() << "Can`t create path for save generated files";
    }
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::generateCodeAs()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбрать путь"),
                                                     _baseDir.absolutePath(),
                                                     QFileDialog::ShowDirsOnly |
                                                     QFileDialog::DontResolveSymlinks);
    _baseDir.setCurrent(dir);
    generateCode();
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::aboutShow()
{
    qDebug() << __PRETTY_FUNCTION__;
    QMessageBox::about(this, tr("О редакторе шрифтов"),
                tr("<p><b>Редактор шрифтов</b> позволяет создать " \
                   "растровые шрифты для использования на экранах МК " \
                   ".</p>" \
                   "<p> Версия 0.0.1</p>"));
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::save()
{
    qDebug() << "Save project: " << _projName;
    QFile saveFile;
    QDir::setCurrent(_projectPath.absolutePath());
    saveFile.setFileName(QStringLiteral("project.json"));
    if(!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning() << "Couldn't open save file.";
        return;
    }
    QJsonObject json;
    json["name"] = _projName;
    json["program"] = programName;
    json["version"] = version;

    _wgtFont->save(json);
    _wgtGlyphs->save(json);

    saveFile.write(QJsonDocument(json).toJson());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::load()
{
    QFile loadFile(QStringLiteral("project.json"));
    QDir::setCurrent(_projectPath.absolutePath());
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open project.");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject json = loadDoc.object();

    QString program;
    if (json.contains("program") && json["program"].isString())
    {
        program = json["program"].toString();
    }
    if(program != programName)
    {
        qWarning() << "The data is not project \"Font Creator\"";
        return;
    }
    if (json.contains("name") && json["name"].isString())
    {
        _projName = json["name"].toString();
        setWindowTitle(_projName);
    }
    if (json.contains("version") && json["version"].isString())
    {

    }

    _wgtFont->load(json);
    _wgtGlyphs->load(json);

}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::settingsShow()
{
    qDebug() << __PRETTY_FUNCTION__;
    if(_settings->exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted";
    }
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------- P R O T E C T E D -----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainWindow::closeEvent: Close event";
    QMainWindow::closeEvent(event);
}

