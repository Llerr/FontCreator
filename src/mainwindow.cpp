#include <QSplitter>

#include "fontWidget.h"
#include "glyphsWidget.h"
#include "editWidget.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    _wgtEdit   = new EditWidget(this);
    _wgtFont   = new FontWidget(this);
    _wgtGlyphs = new GlyphsWidget(this);
    splitter->addWidget(_wgtFont);
    splitter->addWidget(_wgtEdit);
    splitter->addWidget(_wgtGlyphs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

