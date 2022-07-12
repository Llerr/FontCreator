#include <QDebug>
#include <QScrollArea>
#include <QPushButton>

#include "editWidget.h"
#include "ui_editWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::EditWidget)
{
    _ui->setupUi(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    _wgtEdit = new DrawEditWidget(_glyphEdt);
    scrollArea->setWidget(_wgtEdit);
    _ui->verticalLayout->addWidget(scrollArea);

    connect(_ui->btnZoomIn,  qOverload<bool>(&QPushButton::clicked), _wgtEdit, &DrawEditWidget::on_btnZoomIn_clicked);
    connect(_ui->btnZoomOut, qOverload<bool>(&QPushButton::clicked), _wgtEdit, &DrawEditWidget::on_btnZoomOut_clicked);


}

//----------------------------------------------------------------------------------------------------------------------
EditWidget::~EditWidget()
{
    delete _ui;
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P U B L I C   S L O T S ---------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void EditWidget::receiveGlyph(const Glyph &glyph)
{
    qDebug() << "Select glyph: U+" << hex << glyph.key;
    _glyphIn = glyph;
    _glyphEdt = _glyphIn;

    updateEditFields();
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P R I V A T E   S L O T S -------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnCancel_clicked()
{
    qDebug() << __func__;
    _glyphEdt = _glyphIn;
    updateEditFields();
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnOk_clicked()
{
    qDebug() << __func__;
    _glyphIn = _glyphEdt;
    emit editFinished(_glyphIn);
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnZoomOut_clicked()
{

}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnZoomIn_clicked()
{
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtUnicode_editingFinished()
{
    qDebug() << __func__;
    int key = _ui->edtUnicode->text().toInt(nullptr, 16);
    if(key > 0)
        _glyphEdt.key = key;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtXAdvance_editingFinished()
{
    qDebug() << __func__;
    int xAdvance = _ui->edtXAdvance->text().toInt();
    if(xAdvance > 0)
        _glyphEdt.xAdvance = xAdvance;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtWidth_editingFinished()
{
    qDebug() << __func__;
    int width = _ui->edtWidth->text().toInt();
    if(width > 0)
    {
        _glyphEdt.width = width;
        _glyphEdt.img = _glyphEdt.img.copy(0, 0, _glyphEdt.width, _glyphEdt.height);
        _wgtEdit->update();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtHeight_editingFinished()
{
    qDebug() << __func__;
    int height = _ui->edtHeight->text().toInt();
    if(height > 0)
    {
        _glyphEdt.height = height;
        _glyphEdt.img = _glyphEdt.img.copy(0, 0, _glyphEdt.width, _glyphEdt.height);
        _wgtEdit->update();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtDX_editingFinished()
{
    qDebug() << __func__;
    int dX = _ui->edtDX->text().toInt();
    if(dX > 0)
        _glyphEdt.dx = dX;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtDY_editingFinished()
{
    qDebug() << __func__;
    int dY = _ui->edtDY->text().toInt();
    if(dY > 0)
        _glyphEdt.dy = dY;
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------------- P R I V A T E ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void EditWidget::updateEditFields()
{
    _ui->edtUnicode->setText(QString("%1").arg(_glyphEdt.key, 4, 16, QLatin1Char('0')).toUpper());
    _ui->edtXAdvance->setText(QString("%1").arg(_glyphEdt.xAdvance));
    _ui->edtWidth->setText(QString("%1").arg(_glyphEdt.width));
    _ui->edtHeight->setText(QString("%1").arg(_glyphEdt.height));
    _ui->edtDX->setText(QString("%1").arg(_glyphEdt.dx));
    _ui->edtDY->setText(QString("%1").arg(_glyphEdt.dy));

    _wgtEdit->adjustSize();
    _wgtEdit->update();
}

