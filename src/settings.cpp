#include <QDebug>

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
    _highliterPrefix = new Highlighter(_ui->edtPrefix->document());
    _highliterPostfix = new Highlighter(_ui->edtPostfix->document());

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
const QString &Settings::baseFileBody() const
{
    return _baseFileBody;
}

//----------------------------------------------------------------------------------------------------------------------
const QString &Settings::baseFileName() const
{
    return _baseFileName;
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
//--------------------------- P R I V A T E ----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void Settings::saveSettings()
{
    // Основное
    _basePath          = _ui->edtPath->text();
    _baseFileName      = _ui->edtFileName->text();
    _baseFileBody      = _ui->edtFileBody->toPlainText();
    _baseGenPathStruct = _ui->chkGenPathStruct->isChecked();

    _settings.setValue("Base/Path"         , _baseGenPathStruct);
    _settings.setValue("Base/FileName"     , _baseFileName     );
    _settings.setValue("Base/FileBody"     , _baseFileBody     );
    _settings.setValue("Base/GenPathStruct", _basePath         );

    // Содержимое
    _genPrefix  = _ui->edtPrefix->toPlainText();
    _genPointer = _ui->chkPointer->isChecked();
    _genGenFunc = _ui->chkGenFunc->isChecked();
    _genPack    = _ui->chkPack->isChecked();
    _genPostfix = _ui->edtPostfix->toPlainText();
    _settings.setValue("Generation/Prefix",  _genPrefix);
    _settings.setValue("Generation/Pointer", _genPointer  );
    _settings.setValue("Generation/GenFunc", _genGenFunc);
    _settings.setValue("Generation/Pack",    _genPack);
    _settings.setValue("Generation/Postfix",  _genPostfix);

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
    _basePath          = _settings.value("Base/Path").toString();
    _baseFileName      = _settings.value("Base/FileName").toString();
    _baseFileBody      = _settings.value("Base/FileBody").toString();
    _baseGenPathStruct = _settings.value("Base/GenPathStruct").toBool();

    _ui->edtPath->setText(_basePath);
    _ui->edtFileName->setText(_baseFileName);
    _ui->edtFileBody->setPlainText(_baseFileBody);
    _ui->chkGenPathStruct->setChecked(_baseGenPathStruct);


    // Содержимое
    _genPrefix  = _settings.value("Generation/Prefix").toString();
    _genPointer = _settings.value("Generation/Pointer").toBool();
    _genGenFunc = _settings.value("Generation/GenFunc").toBool();
    _genPack    = _settings.value("Generation/Pack").toBool();
    _genPostfix = _settings.value("Generation/Postfix").toString();

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

