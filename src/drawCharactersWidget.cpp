/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QDebug>
#include <QtWidgets>
#include <QPalette>

#include "drawCharactersWidget.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
DrawCharactersWidget::DrawCharactersWidget(QWidget *parent)
    : QWidget(parent),
      _columns(16),
      _lastKey(-1),
      _squareSize(16),
      _startCode(0),
      _endCode(0x10FFFF)
{
    calculateSquareSize();
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::updateFont(const QFont &font)
{
    _displayFont.setFamily(font.family());
    calculateSquareSize();
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::updateSize(const QString &fontSize)
{
    _displayFont.setPointSize(fontSize.toInt());
    calculateSquareSize();
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::updateStyle(const QString &fontStyle)
{
    QFontDatabase fontDatabase;
    const QFont::StyleStrategy oldStrategy = _displayFont.styleStrategy();
    _displayFont = fontDatabase.font(_displayFont.family(), fontStyle, _displayFont.pointSize());
    _displayFont.setStyleStrategy(oldStrategy);
    calculateSquareSize();
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::updateFontMerging(bool enable)
{
    if (enable)
        _displayFont.setStyleStrategy(QFont::PreferDefault);
    else
        _displayFont.setStyleStrategy(QFont::NoFontMerging);
    adjustSize();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::calculateSquareSize()
{
    _squareSize = qMax(16, 4 + QFontMetrics(_displayFont, this).height());
}

//----------------------------------------------------------------------------------------------------------------------
QSize DrawCharactersWidget::sizeHint() const
{
    int numChars = _endCode - _startCode;
    QSize ret = QSize(_columns*_squareSize + 1, ((numChars + _columns)/_columns)*_squareSize + 1); ///< Количество символов
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::setUnicodeGroup(const UnicodeRange &range)
{
//    _numChars = range;
    qDebug() << "Set group: " << range.name << Qt::hex
             << ", start: " << range.start << ", end: " << range.end;
    _startCode = range.start;
    _endCode = range.end;
    resize(sizeHint());
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint widgetPosition = mapFromGlobal(event->globalPos());
    int key = (widgetPosition.y()/_squareSize)*_columns + widgetPosition.x()/_squareSize +_startCode;

    QString text = QString("U+%1").arg(key, 4, 16, QLatin1Char('0')).toUpper();
    emit characterSelectedInfo(text);

    if(event->buttons() == Qt::LeftButton && _lastKey != key)
    {
        uint start = std::min(key, _lastKey);
        uint stop = std::max(key, _lastKey);
        for(uint i = start+1; i < stop +1; ++i)
        {
            if(_keys.contains(i))
                _keys.remove(i);
            else
                _keys.insert(i);
        }
        update();
        _lastKey = key;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _lastKey = (event->y()/_squareSize)*_columns + event->x()/_squareSize +_startCode;
        if((event->modifiers() & Qt::ControlModifier) != Qt::ControlModifier )
        {
            _keys.clear();
        }
        if(_keys.contains(_lastKey))
            _keys.remove(_lastKey);
        else
            _keys.insert(_lastKey);

        if (QChar(_lastKey).category() != QChar::Other_NotAssigned)
            emit characterSelected(QChar(_lastKey));
        update();
    }
    else
        QWidget::mousePressEvent(event);
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    _keys.clear();
    update();
}

//----------------------------------------------------------------------------------------------------------------------
void DrawCharactersWidget::paintEvent(QPaintEvent *event)
{
    QPalette pal = palette();
    QPainter painter(this);
    painter.fillRect(event->rect(), pal.base());
    painter.setFont(_displayFont);

    QRect redrawRect = event->rect();
    int beginRow = redrawRect.top()/_squareSize;
    int endRow = redrawRect.bottom()/_squareSize;
    int beginColumn = redrawRect.left()/_squareSize;
    int endColumn = redrawRect.right()/_squareSize;

    painter.setPen(QPen(Qt::gray));
    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            painter.drawRect(column*_squareSize, row*_squareSize, _squareSize, _squareSize);
        }
    }

    QFontMetrics fontMetrics(_displayFont);
    for (int row = beginRow; row <= endRow; ++row)
    {
        for (int column = beginColumn; column <= endColumn; ++column)
        {
            int key = row*_columns + column + _startCode;

            painter.setClipRect(column*_squareSize, row*_squareSize, _squareSize, _squareSize);

            if (_keys.contains(key))
            {
                painter.fillRect(column*_squareSize + 1, row*_squareSize + 1, _squareSize, _squareSize, pal.highlight());
                painter.setPen(pal.highlightedText().color());
            }
            else
            {
                painter.fillRect(column*_squareSize + 1, row*_squareSize + 1, _squareSize, _squareSize, pal.base());
                painter.setPen(pal.text().color());
            }

            painter.drawText(column*_squareSize + (_squareSize / 2) - fontMetrics.horizontalAdvance(QChar(key))/2,
                             row*_squareSize + 4 + fontMetrics.ascent(),
                             QString(QChar(key)));
        }
    }
}
