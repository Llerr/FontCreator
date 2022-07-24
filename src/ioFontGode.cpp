
#include "settings.h"
#include "ioFontGode.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
IOFontGode::IOFontGode(const GlyphsMap *glyphs, const Settings *sets):
    _glyphs(glyphs),
    _settings(sets)
{

}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::setGlyphs(const GlyphsMap *newGlyphs)
{
    _glyphs = newGlyphs;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::setSettings(const Settings *newSettings)
{
    _settings = newSettings;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::saveFont(const QDir &baseDir, const QString &fontName)

{
    generateBaseFile();
    generateFontHeader(fontName);
    generateFontBody(fontName);
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::generateBaseFile()
{
    qDebug() << "Generate base file: " << _settings;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::generateFontHeader(const QString &fontName)
{
    QString fileName = fontName + ".h";
    qDebug() << "Generate header: " << fileName;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::generateFontBody(const QString &fontName)
{
    QString fileName = fontName + ".cpp";
    qDebug() << "Generate body: " << fileName;
}

