#include <QtWidgets>

#include "drawEditWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
DrawEditWidget::DrawEditWidget(Glyph &glyph, QWidget *parent) :
    QWidget(parent),
    _glyph(glyph),
    _scale(1)
{

}

//----------------------------------------------------------------------------------------------------------------------
QSize DrawEditWidget::sizeHint() const
{
    QWidget *parent = parentWidget();
    QSize retSize = parent->size();
    int width = retSize.width();
    int height = retSize.height();
    retSize.rwidth() = qMax(width, _glyph.width*_scale);
    retSize.rheight() = qMax(height, _glyph.height*_scale);
    return retSize;
}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::on_btnZoomOut_clicked()
{
    _scale -= (_scale < 2)?0:1;
    qDebug() << __func__ << "|width: " << _glyph.width * _scale << ", scale: " << _scale;
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::on_btnZoomIn_clicked()
{
    _scale += (_scale > 101)?0:1;
    qDebug() << __func__ << "|width: " << _glyph.width * _scale << ", scale: " << _scale;
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------- P R O T E C T E D ----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        int width = _glyph.width * _scale;
        int height = _glyph.height * _scale;

        int iX = (event->x() - size().width()/2 + width/2)/_scale;
        int iY = (event->y() - size().height()/2 + height/2)/_scale;

        auto color = _glyph.img.pixelIndex(iX, iY);
        qDebug() << "(" << iX << ", " << iY << ")  - color ("
                 << hex << color << " ), " << _glyph.img.format();

        color = (color)?0:1;
        _glyph.img.setPixel(iX, iY, color);
        update();
    }
    else
        QWidget::mousePressEvent(event);

}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QBrush brush(Qt::white);
    QPen pen(Qt::lightGray);

    painter.setPen(pen);
    painter.setBrush(brush);
    int width = _glyph.width * _scale;
    int height = _glyph.height * _scale;
    int x = size().width()/2 - width/2;
    int y = size().height()/2 - height/2;
    QRect rect(x,y, width, height);
    painter.drawRect(rect);

    painter.setPen(QPen(Qt::black));
    painter.drawImage(rect, _glyph.img);

    if(_scale > 4)
    {
        painter.setPen(QColor(127,127,127,127));
        for (int dx = 0; dx < _glyph.width; ++dx)
        {
            painter.drawLine(x+dx*_scale, y, x+dx*_scale, y+height);
        }
        for (int dy = 0; dy < _glyph.height; ++dy)
        {
            painter.drawLine(x, y + dy*_scale, x + width, y + dy*_scale);
        }
    }

}
