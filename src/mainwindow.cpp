#include <QSplitter>
#include <QDebug>

#include "fontWidget.h"
#include "glyphsWidget.h"
#include "editWidget.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _wgtFont   = new FontWidget(this);
    ui->dockFont->setWidget(_wgtFont);
    _wgtGlyphs = new GlyphsWidget(this);
    ui->dockGlyphs->setWidget(_wgtGlyphs);
    _wgtEdit   = new EditWidget(this);
    setCentralWidget(_wgtEdit);

    connect(_wgtFont, qOverload<const GlyphsMap &, const QString &>(&FontWidget::exportGlyphs),
            _wgtGlyphs, &GlyphsWidget::receiveGlyphs);
}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------- P R O T E C T E D -----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainWindow::closeEvent: Close event";
    QMainWindow::closeEvent(event);
}

