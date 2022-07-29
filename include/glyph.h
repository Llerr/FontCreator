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
    int16_t width;    ///< Ширина символа
    int16_t height;   ///< Высота символа
    int16_t dx;       ///< Смещение левого нижнего угла символа от точки рисования по X
    int16_t dy;       ///< Смещение левого нижнего угла символа от точки рисования по Y
    int16_t xAdvance; ///< Смещение следующего символа по координате X в пикселях
    int16_t yAdvance; ///< Смещение следующего символа по координате Y в пикселях

    uint16_t idx; ///< Индекс глифа в массиве данных шрифта @note Расчитыется во ремя генерации выходного файла
};

typedef QMap<int, Glyph> GlyphsMap;

int getByIndex(GlyphsMap &map, int idx);

//QDebug &operator<<(QDebug &stream, const Glyph &gl);

#endif // GLYPH_H
