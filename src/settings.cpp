#include "settings.h"
#include "highlighter.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Settings)
{
    _ui->setupUi(this);

    _highliter = new Highlighter(_ui->edtFileBody->document());
}

Settings::~Settings()
{
    delete _ui;
}
