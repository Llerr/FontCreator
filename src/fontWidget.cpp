#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>

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
    QScrollArea *scrollArea = new QScrollArea;
    _wgtChars = new CharacterWidget();
    scrollArea->setWidget(_wgtChars);
    ui->verticalLayout->addWidget(scrollArea);
    _wgtChars->updateFontMerging(false);

    on_cmbFont_currentFontChanged(ui->cmbFont->font());

    connect(ui->cmbStyles, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), _wgtChars, &CharacterWidget::updateStyle);
    connect(ui->cmbSizes, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), _wgtChars, &CharacterWidget::updateSize);
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
//    ui->lblFontSize->setText(QString::number(_font.pointSize()));
}

//----------------------------------------------------------------------------------------------------------------------
//--------------------------- P R I V A T E   S L O T S ----------------------------------------------------------------
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
    QString currentSize = ui->cmbSizes->currentText();

    {
        const QSignalBlocker blocker(ui->cmbSizes);
        // sizeCombo signals are now blocked until end of scope
        ui->cmbSizes->clear();

        int size;
        if (fontDatabase.isSmoothlyScalable(font.family(), fontDatabase.styleString(font))) {
            foreach (size, QFontDatabase::standardSizes()) {
                ui->cmbSizes->addItem(QVariant(size).toString());
                ui->cmbSizes->setEditable(true);
            }

        } else {
            foreach (size, fontDatabase.smoothSizes(font.family(), fontDatabase.styleString(font))) {
                ui->cmbSizes->addItem(QVariant(size).toString());
                ui->cmbSizes->setEditable(false);
            }
        }
    }

    int sizeIndex = ui->cmbSizes->findText(currentSize);

    if(sizeIndex == -1)
        ui->cmbSizes->setCurrentIndex(qMax(0, ui->cmbSizes->count() / 3));
    else
        ui->cmbSizes->setCurrentIndex(sizeIndex);
}

//----------------------------------------------------------------------------------------------------------------------
//--------------------------- P R I V A T E   S L O T S ----------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_btnOpenFont_clicked()
{
    qDebug() << "FontWidget::on_btnOpenFont_clicked()";
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Font"),
                                                    "",
                                                    tr("Font (*.ttf);;All (*)"));
    qDebug() << "Open file: " << fileName;
    if(fileName.length() > 0)
    {
        int id = QFontDatabase::addApplicationFont(fileName);
        qDebug() << "New font ID: " << id;
        QString family = QFontDatabase::applicationFontFamilies(id).first();
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
    setFontSize(_font.pointSize());
    findStyles(_font);
    findSizes(_font);
    _wgtChars->updateFont(_font);
}

//----------------------------------------------------------------------------------------------------------------------
void FontWidget::on_cmbStyles_activated(int val)
{
    qDebug() << "on_cmbStyles_changed: " << val;
}
