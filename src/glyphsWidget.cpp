#include <QtWidgets>

#include "glyph.h"
#include "glyphsWidget.h"
#include "qdebug.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
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
int GlyphsWidget::save(QJsonObject &json)
{
    QJsonArray array;
    for(auto &glyph: _glyphs)
    {
        QJsonObject glyphJson;
        glyph.save(glyphJson);
        array.append(glyphJson);
    }
    QJsonObject glypshJson;
    glypshJson["map"] = array;
    glypshJson["name"] = _fontName;
    json["glyphs"] = glypshJson;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
int GlyphsWidget::load(QJsonObject &json)
{
    qDebug() << "Load glyphs";
    GlyphsMap loadedGlyphs;
    QString loadedName;
    if (json.contains("glyphs") && json["glyphs"].isObject())
    {
//        qDebug() << "glyphs";
        QJsonObject glyphsJson = json["glyphs"].toObject();
        if(glyphsJson.contains("map") && glyphsJson["map"].isArray())
        {
//            qDebug() << "map";
            QJsonArray glyphArray = glyphsJson["map"].toArray();

            for (int levelIndex = 0; levelIndex < glyphArray.size(); ++levelIndex)
            {
                QJsonObject glyphJson = glyphArray[levelIndex].toObject();
                Glyph glyph;
                if(glyph.load(glyphJson) == 0)
                {
//                    qDebug() << "Load glyph: " << glyph.key;
                    loadedGlyphs[glyph.key] = glyph;
                }
            }
        }
        if(glyphsJson.contains("name") && glyphsJson["name"].isString())
        {
            loadedName = glyphsJson["name"].toString();
            qDebug() << "Font name for glyphs: " << loadedName;
        }
    }
    _glyphs.clear();
    _wgtDraw->resetSelected();
    receiveGlyphs(loadedGlyphs, loadedName);
    _wgtDraw->updateSize();
    return 0;
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
//        qDebug() << glyph << ", Img pixels: " << glyph.img.pixelIndex(1,1)
//                 << ", " << glyph.img.pixelIndex(1,2);
        _glyphs[glyph.key] = glyph;
        if(glyph.points.size() == 0)
            _glyphs[glyph.key].calcPoints();
    }
    _ui->lblFont->setText(_fontName);
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

