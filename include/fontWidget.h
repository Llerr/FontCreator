#ifndef FONT_H
#define FONT_H

#include <QWidget>

#include "glyph.h"
#include "qvector.h"
#include "settings.h"

class DrawCharactersWidget;
class QLabel;
class QScrollArea;
class Settings;

//----------------------------------------------------------------------------------------------------------------------
namespace Ui
{
class FontWidget;
}

class FontWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FontWidget(QWidget *parent = nullptr);
    ~FontWidget();

    void setFontSize(int size);

    void setSettings(Settings *sets);

    int save(QJsonObject &json);
    int load(QJsonObject &json);

protected:
 void closeEvent(QCloseEvent *event) override;
 void hideEvent(QHideEvent *event) override;

private:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
    void fillUnicodeRanges();
public slots:
    void openFontClick();
    void fontChange(const QFont &font);
    void StyleChange(int idx);
    void fontSizeChange(int idx);
    void unicodeGroupChange(int idx);
    void symbSearchEdit(const QString &text);
    void addGlyphsClick();
    void receiveChar(const QChar &symb);
signals:
    void exportGlyphs(const GlyphsMap &, const QString &);
private:
    Ui::FontWidget *_ui;

    GlyphsMap _glyphs;                    ///< Выбранные глифы
    int _pointSize;                       ///< Текущий размер шрифта
    QFont _font;                          ///< Выбранный шрифт
    DrawCharactersWidget *_wgtChars;      ///< Виджет для отрисовки символов шрифта

    QScrollArea *_scrollArea;  ///< Для прокрутки и отображения всего содержимого _wgtChars
    QImage *_testImage;
    QLabel *_lbl;

    Settings *_settings;
//    QWidget *_wgt;
};

#endif // FONT_H
