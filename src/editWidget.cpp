#include "include/editWidget.h"
#include "ui_editWidget.h"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);
}

EditWidget::~EditWidget()
{
    delete ui;
}
