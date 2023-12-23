#include <QtWidgets>

#include "drawEditWidget.h"
#include "qfont.h"
#include "qnamespace.h"
#include "qpen.h"
#include "qpoint.h"
#include "qrgb.h"
#include "qwidget.h"

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
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::on_btnZoomIn_clicked()
{
    _scale += (_scale > 201)?0:1;
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------- P R O T E C T E D ----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->angleDelta().y() < 0)
        {
            on_btnZoomOut_clicked();
        }
        else
        {
            on_btnZoomIn_clicked();
        }
    }
    else
    {
        QWidget::wheelEvent(event);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::mouseMoveEvent(QMouseEvent *event)
{
    int idx = -1;

    int width = _glyph.width * _scale;
    int height = _glyph.height * _scale;

    int iX = (event->x() - size().width()/2 + width/2)/_scale;
    int iY = (event->y() - size().height()/2 + height/2)/_scale;
    if(iX > _glyph.width || iX < 0 || iY > _glyph.height || iY < 0)
    {
        emit coordChange(QPoint(-1, -1), -1);
        QWidget::mouseMoveEvent(event);
        return;
    }
    QPoint point(iX, iY);
    static QPoint oldPoint;
    if(oldPoint != point)
    {
        oldPoint = point;
        idx = _glyph.points.indexOf(QPoint(iX, iY));
        emit coordChange(point, idx);
//        qDebug() << "[" << iX << ", " << iY << "]" << "width: " << _glyph.width << ", height: " << _glyph.height;
    }
    QWidget::mouseMoveEvent(event);
}

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
        qDebug() << "(" << iX << ", " << iY << ")  - cur color ("
                 << Qt::hex << color << " ), " << _glyph.img.format();

        color = (color)?0:1;
        _glyph.img.setPixel(iX, iY, color);
        qDebug() << "(" << iX << ", " << iY << ")  - set color ("
                 << Qt::hex << color << " ), " << _glyph.img.format();
        update();
    }
    else
    {
        QWidget::mousePressEvent(event);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QBrush brush(Qt::white);

    painter.setBrush(brush);
    int widthGlyph  = _glyph.width * _scale;
    int heightGlyph = _glyph.height * _scale;
    int x = size().width()/2 - widthGlyph/2;
    int y = size().height()/2 - heightGlyph/2;
    QRect rect(x,y, widthGlyph, heightGlyph);
    painter.drawRect(rect);

    _glyph.img.setColor(0, qRgba(0,0,0,0));
    _glyph.img.setColor(1, qRgba(0,0,0,255));
//    painter.setPen(QPen(Qt::black));
    painter.drawImage(rect, _glyph.img);

    if(_scale > 4)
    {
        painter.setPen(QColor(127,127,127,127));
        for (int dx = 0; dx < _glyph.width; ++dx)
        {
            painter.drawLine(x+dx*_scale, y, x+dx*_scale, y+heightGlyph);
        }
        for (int dy = 0; dy < _glyph.height; ++dy)
        {
            painter.drawLine(x, y + dy*_scale, x + widthGlyph, y + dy*_scale);
        }
    }
    if(_scale > 10)
    {
        painter.setPen(QPen(Qt::white));
        QFont font = painter.font();
        font.setPixelSize(10);
        painter.setFont(font);
        for (int dy = 0; dy < _glyph.height; ++dy)
        {
            for (int dx = 0; dx < _glyph.width; ++dx)
            {
                int idx = _glyph.points.indexOf(QPoint(dx,dy));
                if(idx != -1)
                {
                    QRect rect = QRect(x + dx*_scale + 2, y + dy*_scale,
                                       x + (dx + 1)*_scale, y + (dy + 1)*_scale);
                    QString text = QString::number(_glyph.points.indexOf(QPoint(dx, dy)));
//                    painter.drawText(rect, Qt::AlignBottom|Qt::AlignRight, text);
                    painter.drawText(rect, text);
                }
            }
        }
    }

}
