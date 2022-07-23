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
    scrollArea->resize(600,-1);
    _ui->verticalLayout->addWidget(scrollArea);

    connect(_wgtDraw, qOverload<const QString &>(&DrawGlyphsWidget::characterSelectedInfo),
            _ui->lblGlyph, &QLabel::setText);
    connect(_wgtDraw, qOverload<const Glyph &>(&DrawGlyphsWidget::glyphSelected),
            this, qOverload<const Glyph &>(&GlyphsWidget::glyphSelected));

    connect(_ui->btnDel, qOverload<bool>(&QPushButton::clicked), _wgtDraw, &DrawGlyphsWidget::on_btnDel_clicked);
    connect(_ui->btnPlus, qOverload<bool>(&QPushButton::clicked), _wgtDraw, &DrawGlyphsWidget::on_btnPlus_clicked);
    connect(_ui->btnOne, qOverload<bool>(&QPushButton::clicked), _wgtDraw, &DrawGlyphsWidget::on_btnOne_clicked);
    connect(_ui->btnMinus, qOverload<bool>(&QPushButton::clicked), _wgtDraw, &DrawGlyphsWidget::on_btnMinus_clicked);
    resize(600, 1000);
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
    _ui->lblFont->setText(name);
    _wgtDraw->updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
void GlyphsWidget::editFinish(const Glyph &glyph)
{
    _glyphs[glyph.key] = glyph;
    _wgtDraw->updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P R I V A T E   S L O T S -------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------- P R O T E C T E D ------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void GlyphsWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Wheel event. key: " << event->angleDelta();
    if(event->modifiers() == Qt::NoModifier)
    {
        _wgtDraw->wheelEvent(event);
    }
    else
    {
        QWidget::wheelEvent(event);
    }
}

