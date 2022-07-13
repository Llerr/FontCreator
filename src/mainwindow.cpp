#include <QSplitter>
#include <QDebug>
#include <QMessageBox>

#include "fontWidget.h"
#include "glyphsWidget.h"
#include "editWidget.h"

#include "settings.h"
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

    _settings = new Settings(this);

    connect(_wgtFont, qOverload<const GlyphsMap &, const QString &>(&FontWidget::exportGlyphs),
            _wgtGlyphs, &GlyphsWidget::receiveGlyphs);

    connect(_wgtGlyphs, qOverload<const Glyph &>(&GlyphsWidget::glyphSelected),
            _wgtEdit, &EditWidget::receiveGlyph);

    connect(_wgtEdit, qOverload<const Glyph &>(&EditWidget::editFinished),
            _wgtGlyphs, &GlyphsWidget::editFinish);

}

//----------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------- P R I V A T E   S L O T S --------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::on_actAbout_triggered()
{
    qDebug() << __PRETTY_FUNCTION__;
    QMessageBox::about(this, tr("О редакторе шрифтов"),
                tr("<p><b>Редактор шрифтов</b> позволяет создать " \
                   "растровые шрифты для использования на экранах МК " \
                   ".</p>" \
                   "<p> Версия 0.0.1</p>"));
}

//----------------------------------------------------------------------------------------------------------------------
void MainWindow::on_actSettings_triggered()
{
    qDebug() << __PRETTY_FUNCTION__;
    if(_settings->exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted";
    }
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------- P R O T E C T E D -----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainWindow::closeEvent: Close event";
    QMainWindow::closeEvent(event);
}

