#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>
#include <QtWidgets>

#include "characterWidget.h"
#include "fontWidget.h"
#include "ui_fontWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
FontWidget::FontWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontWidget)
{
    ui->setupUi(this);


    qDebug() << "Start font: " << _font << " size: " << _font.pointSize();
    setFontSize(_font.pointSize());
    QScrollArea *scrollArea = new QScrollArea(this);
    _wgtChars = new CharacterWidget();
    scrollArea->setWidget(_wgtChars);
    ui->verticalLayout->addWidget(scrollArea);
    _wgtChars->updateFontMerging(false);

//    _wgt = new QWidget;
//    _wgt->resize(100,100);
    _lbl = new QLabel(this);
    _lbl->resize(150,100);
    _lbl->setWindowFlag(Qt::Dialog, true);
    _lbl->setWindowTitle("Glyph");
    _testImage = new QImage(150,100, QImage::Format_RGB32);
//    _lbl->setPixmap(QPixmap::fromImage(*_testImage));
//    //    _testImage = new QImage(this);
//    _lbl->show();



    connect(ui->cmbStyles, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), _wgtChars, &CharacterWidget::updateStyle);
    connect(ui->cmbSize, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), _wgtChars, &CharacterWidget::updateSize);
    connect(_wgtChars, QOverload<const QString &>::of(&CharacterWidget::characterSelectedInfo), ui->lblSymbolCode, &QLabel::setText);
    connect(_wgtChars, QOverload<const QChar &>::of(&CharacterWidget::characterSelected), this, &FontWidget::receiveChar);
    on_cmbFont_currentFontChanged(ui->cmbFont->font());
}

//----------------------------------------------------------------------------------------------------------------------
FontWidget::~FontWidget()
{
    delete ui;
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
    QString currentItem = ui->cmbStyles->currentText();
    ui->cmbStyles->clear();

    QString style;
    foreach (style, fontDatabase.styles(font.family()))
        ui->cmbStyles->addItem(style);

    int styleIndex = ui->cmbStyles->findText(currentItem);

    if (styleIndex == -1)
        ui->cmbStyles->setCurrentIndex(0);
    else
        ui->cmbStyles->setCurrentIndex(styleIndex);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::findSizes(const QFont &font)
{
    QFontDatabase fontDatabase;
    QString currentSize = ui->cmbSize->currentText();

    {
        const QSignalBlocker blocker(ui->cmbSize);
        // sizeCombo signals are now blocked until end of scope
        ui->cmbSize->clear();

        int size;
        if (fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
            foreach (size, QFontDatabase::standardSizes()) {
                ui->cmbSize->addItem(QVariant(size).toString());
                ui->cmbSize->setEditable(true);
            }

        } else {
            foreach (size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
                ui->cmbSize->addItem(QVariant(size).toString());
                ui->cmbSize->setEditable(false);
            }
        }
    }

    int sizeIndex = ui->cmbSize->findText(currentSize);

    if(sizeIndex == -1)
        ui->cmbSize->setCurrentIndex(qMax(0, ui->cmbSize->count() / 3));
    else
        ui->cmbSize->setCurrentIndex(sizeIndex);
}

//----------------------------------------------------------------------------------------------------------------------
//--------------------------- P R I V A T E   S L O T S ----------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_btnOpenFont_clicked()
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
        QString family = QFontDatabase::applicationFontFamilies(idFont).first();
        qDebug() << "New font family: " << family;
        QFont newFont(family);
        on_cmbFont_currentFontChanged(newFont);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_cmbFont_currentFontChanged(const QFont &font)
{
    qDebug() << "Select font: " << font;
    _font = font;
//    _font.rawName();
    setFontSize(_font.pointSize());
    findStyles(_font);
    findSizes(_font);
    _wgtChars->updateFont(_font);
    _font.setStyleStrategy(QFont::NoFontMerging);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_cmbStyles_currentIndexChanged(const QString &fontStyle)
{
    qDebug() << "on_cmbStyles_changed: " << fontStyle;
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = _font.styleStrategy();
    _font = fontDatabase.font(_font.family(), fontStyle, _font.pointSize());
    _font.setStyleStrategy(oldStrategy);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_cmbSize_currentIndexChanged(const QString &fontSize)
{
    setFontSize(fontSize.toInt());
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
    qDebug() << "Bound rect: " << boundRect << ", xAdvance: " << fm.horizontalAdvance(symb) << ", yAdvance: " << fm.lineSpacing();
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
    _lbl->show();
//    _wgt->show();
//    _wgt->update();
}

