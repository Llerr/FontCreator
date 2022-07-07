#ifndef FONT_H
#define FONT_H

#include <QWidget>

#include "glyph.h"

class CharacterWidget;
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

protected:
 void closeEvent(QCloseEvent *event) override;
 void hideEvent(QHideEvent *event) override;

private:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
private slots:
    void on_btnOpenFont_clicked();
    void on_cmbFont_currentFontChanged(const QFont &font);
    void on_cmbStyles_currentIndexChanged(const QString &fontStyle);
    void on_cmbSize_currentIndexChanged(const QString &fontSize);
    void on_edtSymbSearch_textEdited(const QString &text);
    void on_btnExport_clicked();
    void receiveChar(const QChar &symb);
signals:
    void exportGlyphs(const GlyphsMap &, const QString &);
private:
    Ui::FontWidget *_ui;

    GlyphsMap _glyphs;
    int _pointSize;
    QFont _font;
    CharacterWidget *_wgtChars;

    QScrollArea *_scrollArea;
    QImage *_testImage;
    QLabel *_lbl;
    QWidget *_wgt;
};

#endif // FONT_H
