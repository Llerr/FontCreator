#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "glyph.h"

class EditWidget;
class GlyphsWidget;
class FontWidget;
class Settings;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actAbout_triggered();
    void on_actSettings_triggered();
protected:
 void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *ui;

    FontWidget   *_wgtFont;   ///< Выбор начертания шрифта
    GlyphsWidget *_wgtGlyphs; ///< Выбраные глифы шрифта
    EditWidget   *_wgtEdit;   ///< Редактируемый глиф

    Settings *_settings; ///< Окно настроек

//    GlyphsMap _glyphs;
};
#endif // MAINWINDOW_H
