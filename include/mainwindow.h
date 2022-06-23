#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Edit;
class Glyphs;
class Font;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Font   *_wgtFont;
    Glyphs *_wgtGlyphs;
    Edit   *_wgtEdit;
};
#endif // MAINWINDOW_H
