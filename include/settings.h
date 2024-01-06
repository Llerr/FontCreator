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

//    void setSettings(const QSettings &newSettings);

    const QString &basePath() const;
    const QString &baseFileNameCurrent() const;
    const QString &baseFileName() const;
    const QString &baseFileNameMorph() const;

    const QString &baseFileBodyCurrent() const;
    const QString &baseFileBody() const;
    const QString &baseFileBodyMorph() const;
    bool baseGenPathStruct() const;
    bool baseGenMorphFont() const;
    bool cutCodeGroup() const;

    const QString &genPrefix() const;
    bool genPointer() const;
    bool genGenFunc() const;
    bool genPack() const;
    const QString &genPostfix() const;

public slots:
    void accept() override;
    void selectPath();
private:
    void saveSettings();
    void loadSettings();
private:
    Ui::Settings *_ui;

    Highlighter *_highliterMain;
    Highlighter *_highliterMainMorph;
    Highlighter *_highliterPrefix;
    Highlighter *_highliterPostfix;

    QSettings _settings;

    // Основное
    QString _basePath;
    QString _baseFileName;
    QString _baseFileNameMorph;
    bool _baseGenPathStruct;
    bool _baseGenMorphFont;
    bool _baseCutCodeGroup;      ///< Обрезать группы кодов до существующих символов
    // Содержимое
    QString _baseFileBody;
    QString _baseFileBodyMorph;
    QString _genPrefix;
    bool _genPointer;
    bool _genGenFunc;
    bool _genPack;
    QString _genPostfix;
};

#endif // SETTINGS_H


