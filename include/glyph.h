#ifndef GLYPH_H
#define GLYPH_H

#include <QImage>
#include <QMap>
#include <QDebug>

class Glyph
{
public:
    Glyph();
    friend QDebug operator<<(QDebug stream, const Glyph &gl);

public:
    int key;      ///< Unicode символа
    QImage img;   ///< Изображение символа
    int width;    ///< Ширина символа
    int height;   ///< Высота символа
    int dx;       ///< Смещение левого нижнего угла символа от точки рисования по X
    int dy;       ///< Смещение левого нижнего угла символа от точки рисования по Y
    int xAdvance; ///< Смещение следующего символа по координате X в пикселях
    int yAdvance; ///< Смещение следующего символа по координате Y в пикселях
};

typedef QMap<int, Glyph> GlyphsMap;

//QDebug &operator<<(QDebug &stream, const Glyph &gl);

#endif // GLYPH_H
