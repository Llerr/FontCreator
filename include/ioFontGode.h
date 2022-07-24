#ifndef IOFONTGODE_H
#define IOFONTGODE_H

#include <QString>
#include <QDir>

#include "glyph.h"

class Settings;

class IOFontGode
{
public:
    IOFontGode(const GlyphsMap *glyphs = nullptr, const Settings *sets = nullptr);

    void setGlyphs(const GlyphsMap *newGlyphs);
    void setSettings(const Settings *newSettings);

    void saveFont(const QDir &baseDir, const QString &fontName);

    void generateBaseFile();
    void generateFontHeader(const QString &fontName);
    void generateFontBody(const QString &fontName);

private:
    QDir _baseDir; ///< Базовый путь, где создаётся шрифт

    const GlyphsMap *_glyphs;  ///< Набор глифов для экспорта
    const Settings *_settings; ///< Настройки
};

#endif // IOFONTGODE_H
