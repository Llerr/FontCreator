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
    _baseFileNmae      = _ui->edtFileName->text();
    _baseFileBody      = _ui->edtFileBody->toPlainText();
    _baseGenPathStruct = _ui->chkGenPathStruct->isChecked();
    _settings.setValue("Base/Path"         , _baseGenPathStruct);
    _settings.setValue("Base/FileName"     , _baseFileBody     );
    _settings.setValue("Base/FileBody"     , _baseFileNmae     );
    _settings.setValue("Base/GenPathStruct", _basePath         );

    // Содержимое
    _genPrefix  = _ui->edtPrefix->toPlainText();
    _genPointer = _ui->chkPointer->isChecked();
    _genGenFunc = _ui->chkGenFunc->isChecked();
    _genPack    = _ui->chkPack->isChecked();
    _genPostfix = _ui->edtPostfix->toPlainText();
    _settings.setValue("Generation/Prefix",  _genPostfix);
    _settings.setValue("Generation/Pointer", _genPack   );
    _settings.setValue("Generation/GenFunc", _genGenFunc);
    _settings.setValue("Generation/Pack",    _genPointer);
    _settings.setValue("Generation/Postfix", _genPrefix );

    qDebug() << "Save settings";
}

//----------------------------------------------------------------------------------------------------------------------
void Settings::loadSettings()
{
    // Основное
    _basePath          = _settings.value("Base/Path").toString();
    _baseFileNmae      = _settings.value("Base/FileName").toString();
    _baseFileBody      = _settings.value("Base/FileBody").toString();
    _baseGenPathStruct = _settings.value("Base/GenPathStruct").toBool();

    // Содержимое
    _genPrefix  = _settings.value("Generation/Prefix").toString();
    _genPointer = _settings.value("Generation/Pointer").toBool();
    _genGenFunc = _settings.value("Generation/GenFunc").toBool();
    _genPack    = _settings.value("Generation/Pack").toBool();
    _genPostfix = _settings.value("Generation/Postfix").toString();

    qDebug() << "Load settings";
}
