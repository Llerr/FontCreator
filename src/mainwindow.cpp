#include <QSplitter>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "fontWidget.h"
#include "glyphsWidget.h"
#include "editWidget.h"

#include "settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _wgtFont   = new FontWidget(this);
    _ui->dockFont->setWidget(_wgtFont);
    _wgtGlyphs = new GlyphsWidget(this);
    _ui->dockGlyphs->setWidget(_wgtGlyphs);
    _wgtEdit   = new EditWidget(this);
    setCentralWidget(_wgtEdit);

    _settings = new Settings(this);

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
void MainWindow::generateCode()
{
    qDebug() << "Base dir: " << _baseDir;
    if(_wgtGlyphs->glyphs()->isEmpty())
    {
        qDebug() << "Haven`t glyphs for save code";
        return;
    }

    _generator.saveFont(_wgtGlyphs->fontName());
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::generateCodeAs()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выбрать путь"),
                                                     "./",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
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

