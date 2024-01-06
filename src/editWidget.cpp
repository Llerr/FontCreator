#include <QDebug>
#include <QScrollArea>
#include <QPushButton>

#include "drawEditWidget.h"
#include "editWidget.h"
#include "mainwindow.h"
#include "qmainwindow.h"
#include "qspinbox.h"
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
    connect(_ui->edtDX,qOverload<int>(&QSpinBox::valueChanged), this, &EditWidget::edtDX_changed);
    connect(_ui->edtDY,qOverload<int>(&QSpinBox::valueChanged), this, &EditWidget::edtDY_changed);
    if(nullptr != parent)
    {
        connect(_wgtEdit, SIGNAL(coordChange(QPoint, int)), parent, SLOT(coordChanged(QPoint, int)));
//        connect(_wgtEdit, &DrawEditWidget::coordChange(QPoint), parent, &QMainWindow::coordChanged(QPoint));
//        connect(_wgtEdit, &DrawEditWidget::coordChange, parent, &MainWindow::coordChanged);
    }

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
    qDebug() << "Select glyph: U+" << Qt::hex << glyph.key
             << ", " << glyph.img.pixelIndex(1,1)
             << ", " << glyph.img.pixelIndex(1,2);
    _glyphIn = glyph;
    _glyphEdt = _glyphIn;

    updateEditFields();
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P R I V A T E   S L O T S -------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnCancel_clicked()
{
    _glyphEdt = _glyphIn;
    updateEditFields();
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_btnOk_clicked()
{
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
    int key = _ui->edtUnicode->text().toInt(nullptr, 16);
    if(key > 0)
        _glyphEdt.key = key;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtXAdvance_editingFinished()
{
    int xAdvance = _ui->edtXAdvance->text().toInt();
    if(xAdvance > 0)
        _glyphEdt.xAdvance = xAdvance;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::on_edtWidth_editingFinished()
{
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
    int height = _ui->edtHeight->text().toInt();
    if(height > 0)
    {
        _glyphEdt.height = height;
        _glyphEdt.img = _glyphEdt.img.copy(0, 0, _glyphEdt.width, _glyphEdt.height);
        _wgtEdit->update();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::edtDX_changed(int val)
{
    int dX = val;
    _glyphEdt.dx = dX;
}

//----------------------------------------------------------------------------------------------------------------------
void EditWidget::edtDY_changed(int val)
{
    int dY = val;
    _glyphEdt.dy = dY;
    _wgtEdit->update();
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
    _ui->edtDX->setValue(_glyphEdt.dx);
    _ui->edtDY->setValue(_glyphEdt.dy);

    _wgtEdit->adjustSize();
    _wgtEdit->update();
    _wgtEdit->setMouseTracking((_glyphIn.key == 0)?false:true);
}

