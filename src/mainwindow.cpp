#include <QSplitter>

#include "font.h"
#include "glyphs.h"
#include "edit.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    _wgtEdit   = new Edit(this);
    _wgtFont   = new Font(this);
    _wgtGlyphs = new Glyphs(this);
    splitter->addWidget(_wgtFont);
    splitter->addWidget(_wgtEdit);
    splitter->addWidget(_wgtGlyphs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

