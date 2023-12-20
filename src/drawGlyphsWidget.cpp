#include <QtWidgets>
#include <QDebug>

#include "drawGlyphsWidget.h"
#include "glyph.h"
#include "qnamespace.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
DrawGlyphsWidget::DrawGlyphsWidget(GlyphsMap &glyph, QWidget *parent) :
    QWidget(parent),
    _scale(1),
    _columns(16),
    _squareSize(20),
    _glyphs(glyph)
{
    updateSize();
    setMouseTracking(true);
}

//----------------------------------------------------------------------------------------------------------------------
DrawGlyphsWidget::~DrawGlyphsWidget()
{

}

//----------------------------------------------------------------------------------------------------------------------
QSize DrawGlyphsWidget::sizeHint() const
{
    int sqSize = _squareSize * _scale;
    return QSize(_columns*sqSize + 1, (_glyphs.size()/_columns + 1)*sqSize + 1);
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::updateSize()
{
    calculateSquareSize();
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::calculateSquareSize()
{
    int16_t maxSize = 0;
    for(auto &&glyph:_glyphs)
    {
        maxSize = qMax(maxSize, glyph.height);
    }
    _squareSize = qMax(16, 4 + maxSize);
    //    qDebug() << "Square size: " << _squareSize;
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::resetSelected()
{
    _Idxs.clear();
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------ P R I V A T E   S L O T S -------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::on_btnDel_clicked()
{
    qDebug() << "Delete " << _Idxs.size() << " glyphs";
//    qDebug() << "Before delete size: " << _glyphs.size();
    int cnt = 0;
    QVector<int> rmKeys;
    qDebug() << "rmKeyes size: " << rmKeys.size();
    for(const auto &idx:qAsConst(_Idxs))
    {
        int key = getByIndex(_glyphs, idx);
        qDebug() << ++cnt << ": ix: " << idx << ", key: " << key;
        rmKeys.push_back(key);
        if(_key == key)
        {
            emit glyphSelected(Glyph());
        }
    }
    for (auto &&key:rmKeys)
    {
        _glyphs.remove(key);
    }
//    qDebug() << "After delete size: " << _glyphs.size();
    _Idxs.clear();
    updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::on_btnPlus_clicked()
{
    _scale *= (_scale > 10.) ? 1 : 1.2;
    updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::on_btnOne_clicked()
{
    _scale = 1.0;
    updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::on_btnMinus_clicked()
{
    _scale /= (_scale < 0.2) ? 1 : 1.2;
    updateSize();
}

//----------------------------------------------------------------------------------------------------------------------
//---------------------------- P R O T E C T E D -----------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::wheelEvent(QWheelEvent *event)
{
//    qDebug() << "Wheel event. key: " << event->angleDelta();
    if(event->modifiers() == Qt::NoModifier)
    {
        if(event->angleDelta().y() < 0)
        {
            on_btnMinus_clicked();
        }
        else
        {
            on_btnPlus_clicked();
        }
    }
    else
    {
        QWidget::wheelEvent(event);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint widgetPosition = mapFromGlobal(event->globalPos());
    int sqSize = _squareSize * _scale;
    int idx = (widgetPosition.y()/(sqSize))*_columns + widgetPosition.x()/sqSize;
    if(idx < 0)
    {//        qDebug() << start << "| " << stop;

        return;
    }

    int key = getByIndex(_glyphs, idx);

    if(key > 0)
    {
        QString text = QString("U+%1").arg(key, 4, 16, QLatin1Char('0')).toUpper();
        emit characterSelectedInfo(text);
    }
    if(event->buttons() == Qt::LeftButton && _lastIdx != idx)
    {
        uint start = std::min(idx, _lastIdx);
        uint stop = std::max(idx, _lastIdx);
        for(uint i = start+1; i < stop +1; ++i)
        {
            if(_Idxs.contains(i))
                _Idxs.remove(i);
            else
                _Idxs.insert(i);
        }
        update();
        _lastIdx = idx;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::mousePressEvent(QMouseEvent *event)
{
    int sqSize = _squareSize * _scale;
    if (event->button() == Qt::LeftButton)
    {
        _lastIdx = (event->y()/sqSize)*_columns + event->x()/sqSize;
        int key = getByIndex(_glyphs, _lastIdx);
        if(key < 0)
        {
            return;
        }
        emit glyphSelected(_glyphs[key]);
        _key = key;
        if((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier )
        {
            _Idxs.clear();
        }
        if(_Idxs.contains(_lastIdx))
            _Idxs.remove(_lastIdx);
        else
            _Idxs.insert(_lastIdx);
        update();
    }
    else
        QWidget::mousePressEvent(event);

}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _Idxs.clear();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawGlyphsWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int key;
//    painter.fillRect(event->rect(), QBrush(Qt::white));
    int sqSize = _squareSize * _scale;
    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top()/sqSize;
    int endRow = redrawRect.bottom()/sqSize;
    int beginColumn = redrawRect.left()/sqSize;
    int endColumn = redrawRect.right()/sqSize;

    painter.setPen(QPen(Qt::gray));
    painter.setBrush(QBrush(Qt::white));
    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            if( (column + row*_columns) < _glyphs.size() )
                painter.drawRect(column*sqSize, row*sqSize, sqSize, sqSize);
        }
    }

    for (int row = beginRow; row <= endRow; ++row) {

        for (int column = beginColumn; column <= endColumn; ++column)
        {
            if( (column + row*_columns) >= _glyphs.size() )
                break;
            int idx = row*_columns + column;
            painter.setClipRect(column*sqSize, row*sqSize, sqSize, sqSize);

            if (_Idxs.contains(idx))
                painter.fillRect(column*sqSize + 1, row*sqSize + 1, sqSize, sqSize, QBrush(Qt::lightGray));

            key = getByIndex(_glyphs, idx);
            QImage img = _glyphs[key].img;
            QPoint pointForImage(column*sqSize + (sqSize / 2) - _glyphs[key].width*_scale/2,
                              (row + 1)*sqSize - 4 + _glyphs[key].dy*_scale);
            QSize imgSize(_glyphs[key].width*_scale, _glyphs[key].height*_scale);
            QRect drawRect(pointForImage, imgSize);
            painter.setPen(QPen(Qt::black));
            painter.setBrush(QBrush(Qt::lightGray));
            img.setColor(1, qRgba(0,0,0,0xFF) );
            img.setColor(0, qRgba(0xFF,0xFF,0xFF,0) );
            painter.setPen(Qt::black);
            painter.drawImage(drawRect, img);
        }
    }
}
