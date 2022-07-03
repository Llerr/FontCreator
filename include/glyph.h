#ifndef GLYPH_H
#define GLYPH_H

#include <QImage>

class Glyph
{
public:
    Glyph();

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

#endif // GLYPH_H
