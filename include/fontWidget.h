#ifndef FONT_H
#define FONT_H

#include <QWidget>

class CharacterWidget;

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

private:
    void findStyles(const QFont &font);
    void findSizes(const QFont &font);
private slots:
    void on_btnOpenFont_clicked();
    void on_cmbFont_currentFontChanged(const QFont &font);
    void on_cmbStyles_activated(int val);
private:
    Ui::FontWidget *ui;

    int _pointSize;
    QFont _font;
    CharacterWidget *_wgtChars;
};

#endif // FONT_H
