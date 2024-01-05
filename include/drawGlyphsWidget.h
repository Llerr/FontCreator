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

    friend class GlyphsWidget;
public:
    explicit DrawGlyphsWidget(GlyphsMap &glyph, QWidget *parent = nullptr);
    ~DrawGlyphsWidget();
    QSize sizeHint() const override;

    void updateSize();
    void calculateSquareSize();
    void resetSelected();
public slots:
    void on_btnDel_clicked();
    void on_btnPlus_clicked();
    void on_btnOne_clicked();
    void on_btnMinus_clicked();

signals:
    void characterSelectedInfo(const QString &character);
    void glyphSelected(const Glyph &);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    float _scale;
    int _columns;  ///< Количество столбцов в таблице
    int _rows;     ///< Количество строк в таблице
    int _lastIdx;
    QSet<int> _Idxs;
    int _squareSize;

    int _key = -1; ///< Номер глифа отправленого на редактирование

    GlyphsMap &_glyphs;

};

#endif // DRAW_GLYPHS_H
