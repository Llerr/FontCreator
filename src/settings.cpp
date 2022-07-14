#include "settings.h"
#include "highlighter.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Settings)
{
    _ui->setupUi(this);

    _highliterMain = new Highlighter(_ui->edtFileBody->document());
    _highliterPrefix = new Highlighter(_ui->edtPrefix->document());
    _highliterPostfix = new Highlighter(_ui->edtPostfix->document());
}

Settings::~Settings()
{
    delete _ui;
}
