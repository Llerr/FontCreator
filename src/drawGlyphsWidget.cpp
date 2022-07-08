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
//void CharacterWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint widgetPosition = mapFromGlobal(event->globalPos());
//    uint key = (widgetPosition.y()/_squareSize)*_columns + widgetPosition.x()/_squareSize;

//    QString text = QString("U+%1").arg(key, 4, 16, QLatin1Char('0'));
////    QToolTip::showText(event->globalPos(), text, this);
//    emit characterSelectedInfo(text);

//    if(event->buttons() == Qt::LeftButton && _lastKey != key)
//    {
//        uint start = std::min(key, _lastKey);
//        uint stop = std::max(key, _lastKey);
////        qDebug() << start << "| " << stop;
//        for(uint i = start+1; i < stop +1; ++i)
//        {
//            if(_keys.contains(i))
//                _keys.remove(i);
//            else
//                _keys.insert(i);
//        }
//        update();
//        _lastKey = key;
//    }
//}

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
    int key;
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
//    qDebug() << "Glyph size: " << _glyphs.size();
    for (int row = beginRow; row <= endRow; ++row) {

        for (int column = beginColumn; column <= endColumn; ++column)
        {
            if( (column + row*_columns) >= _glyphs.size() )
                break;
            int idx = row*_columns + column;
            painter.setClipRect(column*_squareSize, row*_squareSize, _squareSize, _squareSize);

            if (_lastKey == idx)
                painter.fillRect(column*_squareSize + 1, row*_squareSize + 1, _squareSize, _squareSize, QBrush(Qt::lightGray));

            key = getByIndex(_glyphs, idx);
            QImage img = _glyphs[key].img;
            img.setColor(1, qRgba(0,0,0,0) );
            QPoint pointForImage(column*_squareSize + (_squareSize / 2) - _glyphs[key].width/2,
                              row*_squareSize + (_squareSize / 2) - _glyphs[key].height/2);
            painter.drawImage(pointForImage, img);
//            qDebug() << key << ": " << glyph;
//            painter.drawText(cpointForImageolumn*_squareSize + (_squareSize / 2) - fontMetrics.horizontalAdvance(QChar(key))/2,
//                             row*_squareSize + 4 + fontMetrics.ascent(),
//                             QString(QChar(key)));
        }
    }
}
