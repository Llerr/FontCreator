#include "font.h"
#include "ui_font.h"

Font::Font(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Font)
{
    ui->setupUi(this);
}

Font::~Font()
{
    delete ui;
}
