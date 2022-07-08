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
    explicit DrawGlyphsWidget(GlyphsMap &glyph, QWidget *parent = nullptr);
    ~DrawGlyphsWidget();
    QSize sizeHint() const override;

    void updateSize();
    void calculateSquareSize();
protected:
//    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    int _columns;  ///< Количество столбцов в таблице
    int _rows;     ///< Количество строк в таблице
    int _lastKey;
    QSet<int> _keys;
    int _squareSize;

    GlyphsMap &_glyphs;

};

#endif // DRAW_GLYPHS_H
