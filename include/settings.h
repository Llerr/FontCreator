#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class Highlighter;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *_ui;

    Highlighter *_highliterMain;
    Highlighter *_highliterPrefix;
    Highlighter *_highliterPostfix;
};

#endif // SETTINGS_H
