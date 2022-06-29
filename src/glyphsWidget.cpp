#include "glyphsWidget.h"
#include "ui_glyphsWidget.h"

GlyphsWidget::GlyphsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GlyphsWidget)
{
    ui->setupUi(this);
}

GlyphsWidget::~GlyphsWidget()
{
    delete ui;
}
