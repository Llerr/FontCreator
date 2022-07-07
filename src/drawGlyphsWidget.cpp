#include <QtWidgets>
#include <QDebug>

#include "drawGlyphsWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
DrawGlyphsWidget::DrawGlyphsWidget(GlyphsMap &glyph, QWidget *parent) :
    QWidget(parent),
    _columns(16),
    _squareSize(20),
    _glyphs(glyph)
{
}

//----------------------------------------------------------------------------------------------------------------------
DrawGlyphsWidget::~DrawGlyphsWidget()
{

}

//----------------------------------------------------------------------------------------------------------------------
QSize DrawGlyphsWidget::sizeHint() const
{
    return QSize(_columns*_squareSize + 1, (_glyphs.size()/_columns + 1)*_squareSize + 1);
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::updateSize()
{
    calculateSquareSize();
    adjustSize();
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------- P R O T E C T E D -----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::calculateSquareSize()
{
    int maxSize = 0;
    for(auto &&glyph:_glyphs)
    {
        maxSize = qMax(maxSize, glyph.height);
    }
    _squareSize = qMax(16, 4 + maxSize);
    qDebug() << "Square size: " << _squareSize;
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::mousePressEvent(QMouseEvent *event)
{
    adjustSize();
    if (event->button() == Qt::LeftButton)
    {
        _lastKey = (event->y()/_squareSize)*_columns + event->x()/_squareSize;
        update();
    }
    else
        QWidget::mousePressEvent(event);

}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    painter.fillRect(event->rect(), QBrush(Qt::white));

    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top()/_squareSize;
    int endRow = redrawRect.bottom()/_squareSize;
    int beginColumn = redrawRect.left()/_squareSize;
    int endColumn = redrawRect.right()/_squareSize;

    painter.setPen(QPen(Qt::gray));
    painter.setBrush(QBrush(Qt::white));
    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            if( (column + row*_columns) < _glyphs.size() )
                painter.drawRect(column*_squareSize, row*_squareSize, _squareSize, _squareSize);
        }
    }

//    QFontMetrics fontMetrics(_displayFont);
//    painter.setPen(QPen(Qt::black));
    for (int row = beginRow; row <= endRow; ++row) {

        for (int column = beginColumn; column <= endColumn; ++column)
        {
            if( (column + row*_columns) >= _glyphs.size() )
                continue;
            int key = row*_columns + column;
            painter.setClipRect(column*_squareSize, row*_squareSize, _squareSize, _squareSize);

            if (_lastKey == key)
                painter.fillRect(column*_squareSize + 1, row*_squareSize + 1, _squareSize, _squareSize, QBrush(Qt::lightGray));

//            painter.drawText(column*_squareSize + (_squareSize / 2) - fontMetrics.horizontalAdvance(QChar(key))/2,
//                             row*_squareSize + 4 + fontMetrics.ascent(),
//                             QString(QChar(key)));
        }
    }
}
