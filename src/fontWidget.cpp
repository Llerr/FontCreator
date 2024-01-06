#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>
#include <QtWidgets>
#include <QRawFont>

#include "drawCharactersWidget.h"
#include "fontWidget.h"
#include "qcombobox.h"
#include "qglobal.h"
#include "qrawfont.h"
#include "qvariant.h"
#include "ui_fontWidget.h"
#include "unicoderanges.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
FontWidget::FontWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::FontWidget),
    _glyphs()
{
    _ui->setupUi(this);


    qDebug() << "Start font: " << _font << " size: " << _font.pointSize();
    setFontSize(_font.pointSize());
    _scrollArea = new QScrollArea(this);
    _wgtChars = new DrawCharactersWidget();
    _scrollArea->setWidget(_wgtChars);
    _ui->verticalLayout->addWidget(_scrollArea);
    _wgtChars->updateFontMerging(false);

    _lbl = new QLabel(this);
    _lbl->resize(150,100);
    _lbl->setWindowFlag(Qt::Dialog, true);
    _lbl->setWindowTitle("Glyph");
    _testImage = new QImage(150,100, QImage::Format_RGB32);

    connect(_ui->btnOpenFont, qOverload<bool>(&QToolButton::clicked), this, &FontWidget::openFontClick);
    connect(_ui->cmbFont, qOverload<const QFont &>(&QFontComboBox::currentFontChanged), this, &FontWidget::fontChange);
    connect(_ui->cmbStyles, qOverload<int>(&QComboBox::currentIndexChanged), this, &FontWidget::StyleChange);
    connect(_ui->cmbFontSize, qOverload<int>(&QComboBox::currentIndexChanged), this, &FontWidget::fontSizeChange);
    connect(_ui->edtSymbSearch, qOverload<const QString &>(&QLineEdit::textEdited), this, &FontWidget::symbSearchEdit);
    connect(_ui->btnAddGlyphs, qOverload<bool>(&QToolButton::clicked), this, &FontWidget::addGlyphsClick);


//    connect(_ui->cmbStyles, qOverload<const QString &>(&QComboBox::currentIndexChanged), _wgtChars, &DrawCharactersWidget::updateStyle);
//    connect(_ui->cmbFontSize, qOverload<const QString &>(&QComboBox::currentIndexChanged), _wgtChars, &DrawCharactersWidget::updateSize);
    connect(_wgtChars, qOverload<const QString &>(&DrawCharactersWidget::characterSelectedInfo), _ui->lblSymbolCode, &QLabel::setText);
    connect(_wgtChars, qOverload<const QChar &>(&DrawCharactersWidget::characterSelected), this, &FontWidget::receiveChar);

    connect(_ui->cmbSymbolRanges, qOverload<int>(&QComboBox::currentIndexChanged), this, &FontWidget::unicodeGroupChange);
    fontChange(_ui->cmbFont->font());
}

//----------------------------------------------------------------------------------------------------------------------
FontWidget::~FontWidget()
{
    delete _ui;
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::setFontSize(int size)
{
    _pointSize = size;
    _font.setPointSize(_pointSize);
//    _font.setPixelSize(_pointSize);
    //    ui->lblFontSize->setText(QString::number(_font.pointSize()));
}

//----------------------------------------------------------------------------------------------------------------------
int FontWidget::save(QJsonObject &json)
{
    json["font"] = _font.toString();

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
int FontWidget::load(QJsonObject &json)
{
    if (json.contains("font") && json["font"].isString())
    {
        QString fontName = json["font"].toString();
        _font.fromString(fontName);
        qDebug() << "Set font: " << _font;
        _pointSize = _font.pointSize();
        fontChange(_font);
        _ui->cmbFont->setCurrentFont(_font);

        int idx = _ui->cmbFontSize->findText(QString::number(_pointSize));
        _ui->cmbFontSize->setCurrentIndex(idx);

        idx = _ui->cmbStyles->findText(_font.styleName());
        _ui->cmbStyles->setCurrentIndex(idx);

        return 0;
    }
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------------- P R O T E C T E D -----------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void FontWidget::closeEvent(QCloseEvent *event)
{
    qDebug() << "Close event";
    Q_UNUSED(event);
    _lbl->close();
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "FontWidget::hideEvent";
    _lbl->close();
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------- P R I V A T E ------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void FontWidget::findStyles(const QFont &font)
{
    QFontDatabase fontDatabase;
    _ui->cmbStyles->clear();

    QString curStyle = font.styleName();
    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        _ui->cmbStyles->addItem(style);

    int styleIndex = _ui->cmbStyles->findText(curStyle);

    if (styleIndex == -1)
        _ui->cmbStyles->setCurrentIndex(0);
    else
        _ui->cmbStyles->setCurrentIndex(styleIndex);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = QString::number(font.pointSize());

    const QSignalBlocker blocker(_ui->cmbFontSize);
    // sizeCombo signals are now blocked until end of scope
    _ui->cmbFontSize->clear();

    int size;
    if (fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font)))
    {
        foreach (size, QFontDatabase::standardSizes())
        {
            _ui->cmbFontSize->addItem(QVariant(size).toString());
            _ui->cmbFontSize->setEditable(true);
        }

    }
    else
    {
        foreach (size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font)))
        {
            _ui->cmbFontSize->addItem(QVariant(size).toString());
            _ui->cmbFontSize->setEditable(false);
        }
    }

    int sizeIndex = _ui->cmbFontSize->findText(currentSize);

    if(sizeIndex == -1)
        _ui->cmbFontSize->setCurrentIndex(qMax(0, _ui->cmbFontSize->count() / 3));
    else
        _ui->cmbFontSize->setCurrentIndex(sizeIndex);
    _wgtChars->updateSize(currentSize);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::fillUnicodeRanges()
{
    auto &ranges = UnicodeRanges::getUnicodeRanges();
    QRawFont rFont = QRawFont::fromFont(_font);
    _ui->cmbSymbolRanges->clear();
    for(const auto &range: ranges)
    {
        UnicodeRange curRange;
        for(int key = range.start; key <= range.end; ++key)
        {
            if(rFont.supportsCharacter(key))
            {
                curRange.name = range.name;
                curRange.end = key;
                if(curRange.start == -1)
                {
                    curRange.start = key;
                }
            }
        }
        if(curRange.end != -1)
        {
            QVariant var;
            var.setValue(curRange);
            _ui->cmbSymbolRanges->addItem(curRange.name, var);
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------
//--------------------------------  S L O T S --------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void FontWidget::openFontClick()
{
    static int idFont = -1;
    qDebug() << "FontWidget::on_btnOpenFont_clicked()";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Font"),
                                                    "",
                                                    tr("Font (*.ttf);;All (*)"));

    qDebug() << "Open file: " << fileName;
    if(fileName.length() > 0)
    {
        if(idFont >= 0)
        {
            QFontDatabase::removeApplicationFont(idFont);
            idFont = -1;
        }
        idFont = QFontDatabase::addApplicationFont(fileName);
        qDebug() << "New font ID: " << idFont;
        QString family = QFontDatabase::applicationFontFamilies(idFont).constFirst();
        qDebug() << "New font family: " << family;
        QFont newFont(family);
        fontChange(newFont);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::fontChange(const QFont &font)
{
    qDebug() << "Select font: " << font;
    _font = font;
//    _font.rawName();
    findStyles(_font);
    findSizes(_font);
    setFontSize(_font.pointSize());
    _wgtChars->updateFont(_font);
    _font.setStyleStrategy(QFont::NoFontMerging);
    fillUnicodeRanges();
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::StyleChange(int idx)
{
    if(idx < 0)
        return;
    QString fontStyle = _ui->cmbStyles->itemText(idx);
    qDebug() << "on_cmbStyles_changed: " << fontStyle;
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = _font.styleStrategy();
    _font = fontDatabase.font(_font.family(), fontStyle, _font.pointSize());
    _font.setStyleStrategy(oldStrategy);

    _wgtChars->updateStyle(fontStyle);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::fontSizeChange(int idx)
{
    QString fontSize = _ui->cmbFontSize->itemText(idx);
    setFontSize(fontSize.toInt());

    _wgtChars->updateSize(fontSize);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::unicodeGroupChange(int idx)
{
    UnicodeRange range = _ui->cmbSymbolRanges->currentData().value<UnicodeRange>();
    qDebug() << "Select group: " << idx << ", " << Qt::hex
             << range.name << ", start: " << range.start << ", end: " << range.end;
    UnicodeRange range1 = UnicodeRanges::getRange(_ui->cmbSymbolRanges->currentText());
    qDebug() << Qt::hex << range1.name << ", start: " << range1.start << ", end: " << range1.end;
    _wgtChars->setUnicodeGroup(range);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::symbSearchEdit(const QString &text)
{
    int key = 0;
    bool ok = false;
    switch (text.length())
    {
    case 0:
        break;
    case 1:
        key = text.at(0).unicode();
        break;
    default:
        key = text.toInt(&ok, 16);
    }
    int squareSize = _wgtChars->squareSize();
    int columns = _wgtChars->columns();
    _scrollArea->verticalScrollBar()->setValue(squareSize*key/columns);
    qDebug() << "Text for search: " << text << ", code: " << Qt::hex;
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::addGlyphsClick()
{
    auto & keys = _wgtChars->getKeys();
    QString fontName = _ui->cmbFont->currentText() +
            _ui->cmbStyles->currentText() + _ui->cmbFontSize->currentText();
    fontName.remove(QChar(' '));
    QFontMetrics fm(_font);

    qDebug() << "Export glyphs" << _wgtChars->getKeys().size() << ", font name: " << fontName;
    _glyphs.clear();
    for(auto &&key:keys)
    {
        QChar symb(key);
        QRect boundRect = fm.boundingRect(QString(symb));
        Glyph glyph;
        QImage img(boundRect.size(), QImage::Format_Mono);
        qDebug() << "key: " << key << " " << QString(symb) << boundRect;
        qDebug() << "img: " << img;
        img.setColor(1, qRgba(0,0,0,255));
        img.setColor(0, qRgba(255,255,255,255));
        img.fill(0);
        QPainter painter(&img);

        painter.setFont(_font);
        painter.setPen(Qt::black);

        painter.drawText(-boundRect.topLeft(), QString(symb));
        painter.end();
//        img.save(QString::number(key,16) + ".xpm");
        glyph.key = key;
        glyph.img = img;
        glyph.width = boundRect.size().width();
        glyph.height = boundRect.size().height();
        glyph.dx = boundRect.topLeft().rx();
        glyph.dy = boundRect.topLeft().ry();
        glyph.xAdvance = fm.horizontalAdvance(symb);
        glyph.yAdvance = fm.lineSpacing();
        glyph.baseLine = fm.ascent();
//        qDebug() << "descent: " << fm.descent() << "ascent: " << fm.ascent();

        _glyphs[key] = glyph;
    }
    emit exportGlyphs(_glyphs, fontName);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::receiveChar(const QChar &symb)
{
    const int dy = 80;
    const int dx = 20;
    QFontMetrics fm(_font);

    qDebug() << "Select symb: " << QString(symb) << ", size: " << _pointSize;
    _testImage->fill(Qt::white);
    QPainter painter(_testImage);

    QRect boundRect = fm.boundingRect(symb);
//    int aligin = fm.ascent();
    qDebug() << "Bound rect: " << boundRect
             << ", xAdvance: " << fm.horizontalAdvance(symb)
             << ", yAdvance: " << fm.lineSpacing()
             << ", ascent: " << fm.ascent();
    boundRect.translate(dx, dy);
    painter.setPen(Qt::gray);
    painter.drawRect(boundRect);

    if(fm.inFont(symb))
    {
        painter.setPen(Qt::black);
        painter.setFont(_font);
        painter.drawText(dx, dy, QString(symb));
    }
    QPen coordPen(Qt::gray);
    coordPen.setStyle(Qt::DotLine);
    painter.setPen(coordPen);
    painter.drawLine(0,dy,100,dy);
    painter.drawLine(dx,0,dx,100);

    painter.setPen(Qt::gray);
    _testImage->save("gliph.png");
    _lbl->setPixmap(QPixmap::fromImage(*_testImage));
//    _lbl->show();
//    _wgt->show();
//    _wgt->update();
}

