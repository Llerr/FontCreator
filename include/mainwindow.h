#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDir>

#include "glyph.h"
#include "ioFontCode.h"

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

    Settings *settings() const;

private slots:
    void generateCode();
    void generateCodeAs();
    void settingsShow();
    void aboutShow();
protected:
 void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *_ui;

    FontWidget   *_wgtFont;   ///< Выбор начертания шрифта
    GlyphsWidget *_wgtGlyphs; ///< Выбраные глифы шрифта
    EditWidget   *_wgtEdit;   ///< Редактируемый глиф

    Settings *_settings; ///< Окно настроек

    QDir _baseDir;
    IOFontCode _generator; ///< Класс создания кода шрифта
};
#endif // MAINWINDOW_H
