#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

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

    void setSettings(const QSettings &newSettings);

public slots:
    void accept() override;
private:
    void saveSettings();
    void loadSettings();
private:
    Ui::Settings *_ui;

    Highlighter *_highliterMain;
    Highlighter *_highliterPrefix;
    Highlighter *_highliterPostfix;

    QSettings _settings;
    // Основное
    QString _basePath;
    QString _baseFileNmae;
    QString _baseFileBody;
    bool _baseGenPathStruct;
    // Содержимое
    QString _genPrefix;
    bool _genPointer;
    bool _genGenFunc;
    bool _genPack;
    QString _genPostfix;
};

#endif // SETTINGS_H
