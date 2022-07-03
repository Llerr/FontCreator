#ifndef FONT_H
#define FONT_H

#include <QWidget>

class CharacterWidget;
class QLabel;

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
    void receiveChar(const QChar &symb);
private:
    Ui::FontWidget *ui;

    int _pointSize;
    QFont _font;
    CharacterWidget *_wgtChars;

    QImage *_testImage;
    QLabel *_lbl;
    QWidget *_wgt;
};

#endif // FONT_H
