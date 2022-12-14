#ifndef IOFONTGODE_H
#define IOFONTGODE_H

#include <QString>
#include <QDir>

#include "glyph.h"

class Settings;

class IOFontGode
{
public:
    IOFontGode(GlyphsMap *glyphs = nullptr, const Settings *sets = nullptr);

    void setGlyphs(GlyphsMap *newGlyphs);
    void setSettings(const Settings *newSettings);
    void setBaseDir(const QDir &newBaseDir);

    void saveFont(const QDir &baseDir, const QString &fontName);

    void generateBaseFile();
    void generateFontHeader(const QString &fontName);
    void generateFontBody(const QString &fontName);
protected:
    QDir filePath(const QString &&dir);
    void outImage(QTextStream &out, QImage &img, uint16_t &idx);
    /// Вывод байта в 16-ричной системе.
    /// Добавление префикса 0x, при длинной строке, добавление переноса
    QTextStream &outHexByte(QTextStream &out, uint8_t byte, int &numInLine, int Numberlen = 2);

private:
    QDir _basePath; ///< Базовый путь, где создаётся шрифт

    GlyphsMap *_glyphs;  ///< Набор глифов для экспорта
    const Settings *_settings; ///< Настройки
};

#endif // IOFONTGODE_H
