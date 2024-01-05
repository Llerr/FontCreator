#include <QtWidgets>
#include <QFont>
#include <QPoint>
#include "QInputDialog"

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

    int iX = event->x() - size().width()/2 + width/2;
    int iY = event->y() - size().height()/2 + height/2;
    if(iX >= 0 && iY >= 0)
    {
        iX = iX/_scale;
        iY = iY/_scale;
    }
    static QPoint oldPoint;
    QPoint point(iX, iY);
    if(oldPoint != point)
    {
        oldPoint = point;
        if(iX > (_glyph.width - 1) || iX < 0 || iY > (_glyph.height - 1) || iY < 0)
        {
            emit coordChange(QPoint(-1, -1), -1);
            QWidget::mouseMoveEvent(event);
            return;
        }
        oldPoint = point;
        idx = _glyph.points.indexOf(QPoint(iX, iY));
        emit coordChange(point, idx);
    }
    QWidget::mouseMoveEvent(event);
}//        qDebug() << glyph << ", Img pixels: " << glyph.img.pixelIndex(1,1)
//                 << ", " << glyph.img.pixelIndex(1,2);


//----------------------------------------------------------------------------------------------------------------------
void DrawEditWidget::mousePressEvent(QMouseEvent *event)
{
    int width = _glyph.width * _scale;
    int height = _glyph.height * _scale;

    int iX = event->x() - size().width()/2 + width/2;
    int iY = event->y() - size().height()/2 + height/2;
    if(iX >= 0 && iY >= 0)
    {
        iX = iX/_scale;
        iY = iY/_scale;
    }

    if(iX > (_glyph.width - 1) || iX < 0 || iY > (_glyph.height - 1) || iY < 0)
    {
        return;
    }

    if (event->button() == Qt::LeftButton)
    {

        auto color = _glyph.img.pixelIndex(iX, iY);
        qDebug() << "(" << iX << ", " << iY << ")  - cur color ("
                 << Qt::hex << color << " ), " << _glyph.img.format();

        color = (color)?0:1;
        _glyph.img.setPixel(iX, iY, color);
        if(color)
        {
            _glyph.points.push_back(QPoint(iX, iY));
        }
        else
        {
//            qDebug() << "Length: " << _glyph.points.length();
            int idx = _glyph.points.indexOf(QPoint(iX, iY));
            _glyph.points.remove(idx);
//            qDebug() << "Length: " << _glyph.points.length() << ", idx: " << idx;
        }
        qDebug() << "(" << iX << ", " << iY << ")  - set color ("
                 << Qt::hex << color << " ), " << _glyph.img.format();
        update();
    }
    else if(event->button() == Qt::RightButton && MainWindow::settings()->baseGenMorphFont())
    {
        auto color = _glyph.img.pixelIndex(iX, iY);
        int idx = _glyph.points.indexOf(QPoint(iX, iY));
        if(color)
        {
            int val = QInputDialog::getInt(this, "idx input", "индекс", idx, 0,
                                           _glyph.points.size() - 1, 1, nullptr, Qt::Popup);
            _glyph.points.swapItemsAt(idx, val);
            update();
        }
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
    QPalette pal = palette();
    QPainter painter(this);

    QBrush brush(pal.base());

    painter.setBrush(brush);
    int widthGlyph  = _glyph.width * _scale;
    int heightGlyph = _glyph.height * _scale;
    int x = size().width()/2 - widthGlyph/2;
    int y = size().height()/2 - heightGlyph/2;
    QRect rect(x,y, widthGlyph, heightGlyph);
    painter.drawRect(rect);

    _glyph.img.setColor(0, qRgba(0,0,0,0));
    _glyph.img.setColor(1, pal.text().color().rgba());

    painter.drawImage(rect, _glyph.img);

    if(_scale > 4)
    {
        painter.setPen(Qt::gray);
        for (int dx = 0; dx < _glyph.width; ++dx)
        {
            painter.drawLine(x+dx*_scale, y, x+dx*_scale, y+heightGlyph);
        }
        for (int dy = 0; dy < _glyph.height; ++dy)
        {
            painter.setPen((dy == (-_glyph.dy))?QPen(Qt::red, 2):QPen(Qt::gray));
            painter.drawLine(x, y + dy*_scale, x + widthGlyph, y + dy*_scale);
        }
    }
    if(_scale > 10 && MainWindow::settings()->baseGenMorphFont())
    {
//        painter.setPen(QPen(Qt::white));
        painter.setPen(pal.brightText().color());
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
                    painter.drawText(rect, text);
                }
            }
        }
    }

}
