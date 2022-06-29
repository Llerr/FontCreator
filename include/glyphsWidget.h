#ifndef GLYPHS_H
#define GLYPHS_H

#include <QWidget>

namespace Ui {
class GlyphsWidget;
}

class GlyphsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlyphsWidget(QWidget *parent = nullptr);
    ~GlyphsWidget();

private:
    Ui::GlyphsWidget *ui;
};

#endif // GLYPHS_H
