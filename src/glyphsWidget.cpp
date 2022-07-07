#include <QtWidgets>

#include "glyphsWidget.h"
#include "ui_glyphsWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
GlyphsWidget::GlyphsWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GlyphsWidget),
    _glyphs()

{
    _ui->setupUi(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    _wgtDraw = new DrawGlyphsWidget(_glyphs);
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

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P U B L I C S   S L O T S -------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void GlyphsWidget::receiveGlyphs(const GlyphsMap &glyphs, const QString &name)
{
    qDebug() << "Receive signal of import data: " << glyphs.size() << ", name: " << name;
    _fontName = name;
    for(auto &&glyph : glyphs)
    {
        qDebug() << glyph;
        _glyphs[glyph.key] = glyph;
    }
    _wgtDraw->updateSize();
}
