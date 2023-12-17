#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDir>

#include "glyph.h"
#include "ioFontCode.h"
#include "qdir.h"

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
    void openProject();
    void saveProject();
    void generateCode();
    void generateCodeAs();
    void settingsShow();
    void aboutShow();
protected:
    void save();
    void load();
    void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindow *_ui;

    FontWidget   *_wgtFont;   ///< Выбор начертания шрифта
    GlyphsWidget *_wgtGlyphs; ///< Выбраные глифы шрифта
    EditWidget   *_wgtEdit;   ///< Редактируемый глиф

    Settings *_settings; ///< Окно настроек

    QDir _baseDir;
    IOFontCode _generator; ///< Класс создания кода шрифта

    QDir _projectPath; ///< Имя директории для сохранения проекта
    QString _projName; ///< Имя проекта
};
#endif // MAINWINDOW_H
