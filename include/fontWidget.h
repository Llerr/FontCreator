#ifndef FONT_H
#define FONT_H

#include <QWidget>

#include "glyph.h"

class DrawCharactersWidget;
class QLabel;
class QScrollArea;

namespace Ui {
class FontWidget;
}

class FontWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FontWidget(QWidget *parent = nullptr);
    ~FontWidget();

    void setFontSize(int size);

    int save(QJsonObject &json);
    int load(QJsonObject &json);

protected:
 void closeEvent(QCloseEvent *event) override;
 void hideEvent(QHideEvent *event) override;

private:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
public slots:
    void openFontClick();
    void fontChange(const QFont &font);
    void StyleChange(int idx);
    void fontSizeChange(int idx);
    void symbSearchEdit(const QString &text);
    void addGlyphsClick();
    void receiveChar(const QChar &symb);
signals:
    void exportGlyphs(const GlyphsMap &, const QString &);
private:
    Ui::FontWidget *_ui;

    GlyphsMap _glyphs;
    int _pointSize;
    QFont _font;
    DrawCharactersWidget *_wgtChars;

    QScrollArea *_scrollArea;
    QImage *_testImage;
    QLabel *_lbl;
    QWidget *_wgt;
};

#endif // FONT_H
