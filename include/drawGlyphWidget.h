#ifndef DRAW_GLYPHS_H
#define DRAW_GLYPHS_H

#include <QWidget>
#include <QMap>

#include "glyph.h"

///
/// \brief class GlyphsWidget отображает выбранные символы (глифы)
///
class DrawGlyphsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawGlyphsWidget(QWidget *parent = nullptr);
    ~DrawGlyphsWidget();

protected:
//    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:

};

#endif // DRAW_GLYPHS_H
