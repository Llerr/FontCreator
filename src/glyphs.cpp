#include "glyphs.h"
#include "ui_glyph.h"

Glyphs::Glyphs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Glyphs)
{
    ui->setupUi(this);
}

Glyphs::~Glyphs()
{
    delete ui;
}
