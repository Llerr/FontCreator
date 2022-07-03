#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EditWidget;
class GlyphsWidget;
class FontWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
 void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

    FontWidget   *_wgtFont;
    GlyphsWidget *_wgtGlyphs;
    EditWidget   *_wgtEdit;
};
#endif // MAINWINDOW_H
