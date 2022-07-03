#include <QtWidgets>

#include "glyphsWidget.h"
#include "ui_glyphsWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
GlyphsWidget::GlyphsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GlyphsWidget)
{
    _ui->setupUi(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    _wgtDraw = new DrawGlyphsWidget();
    scrollArea->setWidget(_wgtDraw);
    _ui->verticalLayout->addWidget(scrollArea);

}

//----------------------------------------------------------------------------------------------------------------------
GlyphsWidget::~GlyphsWidget()
{
    delete _ui;
}

//----------------------------------------------------------------------------------------------------------------------
void GlyphsWidget::removeGlyph(int key)
{
    _glyphs.remove(key);
}

//----------------------------------------------------------------------------------------------------------------------
void GlyphsWidget::setGlyph(int key, Glyph glyph)
{
    _glyphs[key] = glyph;
}
