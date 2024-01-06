#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

#include "settings.h"
#include "highlighter.h"
#include "ui_settings.h"

#define SETTINGS_NAME "FontCreator"
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Settings),
    _settings(SETTINGS_NAME)
{
    _ui->setupUi(this);

    _highliterMain = new Highlighter(_ui->edtFileBody->document());
    _highliterMainMorph = new Highlighter(_ui->edtFileBodyMorph->document());
    _highliterPrefix = new Highlighter(_ui->edtPrefix->document());
    _highliterPostfix = new Highlighter(_ui->edtPostfix->document());

    connect(_ui->btnSelectPath, qOverload<>(&QPushButton::pressed), this, &Settings::selectPath);

    if(_settings.allKeys().size() > 0)
    {
        loadSettings();
    }
    else
    {
        saveSettings();
    }
}

//----------------------------------------------------------------------------------------------------------------------
Settings::~Settings()
{
    delete _ui;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::genPostfix() const
{
    return _genPostfix;
}

//----------------------------------------------------------------------------------------------------------------------
bool Settings::genPack() const
{
    return _genPack;
}

//----------------------------------------------------------------------------------------------------------------------
bool Settings::genGenFunc() const
{
    return _genGenFunc;
}

//----------------------------------------------------------------------------------------------------------------------
bool Settings::genPointer() const
{
    return _genPointer;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::genPrefix() const
{
    return _genPrefix;
}

//----------------------------------------------------------------------------------------------------------------------
bool Settings::baseGenPathStruct() const
{
    return _baseGenPathStruct;
}

//----------------------------------------------------------------------------------------------------------------------
bool Settings::baseGenMorphFont() const
{
    return _baseGenMorphFont;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileBodyCurrent() const
{
    if(_baseGenMorphFont)
    {
        return _baseFileBodyMorph;
    }
    else
    {
        return _baseFileBody;
    }
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileBody() const
{
    return _baseFileBody;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileBodyMorph() const
{
    return _baseFileBodyMorph;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileNameCurrent() const
{
    if(_baseGenMorphFont)
    {
        return _baseFileNameMorph;
    }
    else
    {
        return _baseFileName;
    }
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileName() const
{
    return _baseFileName;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileNameMorph() const
{
    return _baseFileNameMorph;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::basePath() const
{
    return _basePath;
}

//----------------------------------------------------------------------------------------------------------------------
//-------------------------- P U B L I C   S L O T S -------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Settings::accept()
{
    qDebug() << __PRETTY_FUNCTION__;
    saveSettings();
    QDialog::accept();
}

//----------------------------------------------------------------------------------------------------------------------
void Settings::selectPath()
{
    QString dirStr = QFileDialog::getExistingDirectory(this, tr("Выбрать базовый путь"),
                                                     _basePath,
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    qDebug() << "Base path: " << dirStr;
    if(dirStr.length() > 0)
    {
        _basePath = dirStr;
    }
    _ui->edtPath->setText(_basePath);
}

//----------------------------------------------------------------------------------------------------------------------
//--------------------------- P R I V A T E ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Settings::saveSettings()
{
    // Основное
    _basePath           = _ui->edtPath->text();
    _baseFileName       = _ui->edtFileName->text();
    _baseFileNameMorph  = _ui->edtFileNameMorph->text();
    _baseGenPathStruct  = _ui->chkGenPathStruct->isChecked();
    _baseGenMorphFont = _ui->chkGenPixelCoords->isChecked();

    _settings.setValue("Base/Path",           _basePath         );
    _settings.setValue("Base/FileName",       _baseFileName     );
    _settings.setValue("Base/FileNameMorph",  _baseFileNameMorph);
    _settings.setValue("Base/GenPathStruct",  _baseGenPathStruct);
    _settings.setValue("Base/GenPixelCoords", _baseGenPathStruct);

    // Содержимое
    _baseFileBody      = _ui->edtFileBody->toPlainText();
    _baseFileBodyMorph = _ui->edtFileBodyMorph->toPlainText();
    _genPrefix         = _ui->edtPrefix->toPlainText();
    _genPointer        = _ui->chkPointer->isChecked();
    _genGenFunc        = _ui->chkGenFunc->isChecked();
    _genPack           = _ui->chkPack->isChecked();
    _genPostfix        = _ui->edtPostfix->toPlainText();

    _settings.setValue("Generation/FileBody",      _baseFileBody);
    _settings.setValue("Generation/FileBodyMorph", _baseFileBodyMorph);
    _settings.setValue("Generation/Prefix",       _genPrefix);
    _settings.setValue("Generation/Pointer",      _genPointer  );
    _settings.setValue("Generation/GenFunc",      _genGenFunc);
    _settings.setValue("Generation/Pack",         _genPack);
    _settings.setValue("Generation/Postfix",      _genPostfix);

    qDebug() << "Save settings";

    qDebug() << "Prefix: " << genPrefix();
    qDebug() << " Generate pointer: " << genPointer();
    qDebug() << " Generate function: " << genGenFunc();
    qDebug() << " Pack data: " << genPack();
    qDebug() << "Postfix: " << genPostfix();
}

//----------------------------------------------------------------------------------------------------------------------
void Settings::loadSettings()
{
    // Основное
    _basePath           = _settings.value("Base/Path").toString();
    _baseFileName       = _settings.value("Base/FileName").toString();
    _baseFileNameMorph  = _settings.value("Base/FileNameMorph").toString();
    _baseGenPathStruct  = _settings.value("Base/GenPathStruct").toBool();
    _baseGenMorphFont = _settings.value("Base/GenPixelCoords").toBool();

    _ui->edtPath->setText(_basePath);
    _ui->edtFileName->setText(_baseFileName);
    _ui->edtFileNameMorph->setText(_baseFileNameMorph);
    _ui->chkGenPathStruct->setChecked(_baseGenPathStruct);
    _ui->chkGenPixelCoords->setChecked(_baseGenMorphFont);


    // Содержимое
    _baseFileBody      = _settings.value("Generation/FileBody").toString();
    _baseFileBodyMorph = _settings.value("Generation/FileBodyMorph").toString();
    _genPrefix         = _settings.value("Generation/Prefix").toString();
    _genPointer        = _settings.value("Generation/Pointer").toBool();
    _genGenFunc        = _settings.value("Generation/GenFunc").toBool();
    _genPack           = _settings.value("Generation/Pack").toBool();
    _genPostfix        = _settings.value("Generation/Postfix").toString();

    _ui->edtFileBody->setPlainText(_baseFileBody);
    _ui->edtFileBodyMorph->setPlainText(_baseFileBodyMorph);
    _ui->edtPrefix->setPlainText(_genPrefix);
    _ui->chkPointer->setChecked(_genPointer);
    _ui->chkGenFunc->setChecked(_genGenFunc);
    _ui->chkPack->setChecked(_genPack);
    _ui->edtPostfix->setPlainText(_genPostfix);

    qDebug() << "Load settings";

    qDebug() << "Prefix: " << genPrefix();
    qDebug() << " Generate pointer: " << genPointer();
    qDebug() << " Generate function: " << genGenFunc();
    qDebug() << " Pack data: " << genPack();
    qDebug() << "Postfix: " << genPostfix();
}

