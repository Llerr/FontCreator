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
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtXAdvance_editingFinished()
{
    qDebug() << __func__;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtWidth_editingFinished()
{
    qDebug() << __func__;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtHeight_editingFinished()
{
    qDebug() << __func__;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtDX_editingFinished()
{
    qDebug() << __func__;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtDY_editingFinished()
{
    qDebug() << __func__;
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

