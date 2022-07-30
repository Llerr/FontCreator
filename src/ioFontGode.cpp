
#include "settings.h"
#include "ioFontGode.h"

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
IOFontGode::IOFontGode(GlyphsMap *glyphs, const Settings *sets):
    _glyphs(glyphs),
    _settings(sets)
{

}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::setGlyphs(GlyphsMap *newGlyphs)
{
    _glyphs = newGlyphs;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::setSettings(const Settings *newSettings)
{
    _settings = newSettings;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::setBaseDir(const QDir &newBaseDir)
{
    _basePath = newBaseDir;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::saveFont(const QDir &baseDir, const QString &fontName)
{
    setBaseDir(baseDir);

    generateBaseFile();
    generateFontHeader(fontName);
    generateFontBody(fontName);
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::generateBaseFile()
{
    qDebug() << "Generate base file: " << _settings->baseFileName();
    if(_settings->baseFileName().isEmpty())
    {
        return;
    }
    QFile file(_basePath.filePath(_settings->baseFileName()));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out << _settings->baseFileBody();
    qDebug() << "Base file saved.";
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::generateFontHeader(const QString &fontName)
{
    QDir path = filePath("include");
    qDebug() << "Generate body: " << path.path() << fontName;
    QFile file(path.filePath(fontName+".h"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);

    out << "#ifndef _" << fontName.toUpper() << "_FONT_H_\n";
    out << "#define _" << fontName.toUpper() << "_FONT_H_\n\n";

    out << "#include \" " << _settings->baseFileName() << "\"\n\n";
    if(_settings->genGenFunc())
    {
        out << "#ifdef __cplusplus\n"
               "extern \"C\" {\n"
               "#endif\n\n";

        out << "dgx_font_t *" << fontName << "();\n\n";
        out << "#ifdef __cplusplus\n"
               "}\n"
               "#endif\n\n";
    }
    else
    {
        out << "extern const dgx_font_t " << fontName << ";\n\n";
    }

    out << "#endif // _" << fontName.toUpper() << "_FONT_H_\n";
}

void IOFontGode::generateFontBody(const QString &fontName)
{

    QDir path = filePath("src");
    qDebug() << "Generate body: " << path.path() << fontName;
    QFile file(path.filePath(fontName+".cpp"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out << _settings->genPrefix() << "\n\n";
    out << "#include \" " << _settings->baseFileName() << "\"\n\n";

    Glyph glyph = _glyphs->first();



    uint16_t idx = 0;
    int16_t yAdvance = glyph.yAdvance;
    int16_t yOffsetLowest = glyph.dy;
    int16_t xWidest = glyph.width;
    int16_t xWidthAverage = glyph.width;
    int dXWidth = 0;

    //---------------------------------- Генерация массива данных ------------------------------------------------------
    out << "// font bitmaps\n";
    out << "static const uint8_t bitmaps[] =\n"
           "{\n";
    // Пройдёмся по всем глифам
    for(auto &glyph: *_glyphs)
    {
        // Вычислим глобальные характеристики шрифта
        yOffsetLowest = qMin(yOffsetLowest, glyph.dy);
        xWidest = qMax(xWidest, glyph.width);
        dXWidth += glyph.xAdvance - xWidthAverage;

        glyph.idx = idx;
        out << QString("// U+%1   ").arg(glyph.key, 4, 16, QChar('0')) << "'" << QString(QChar(glyph.key)) << "' ["
            << glyph.idx <<"]\n";
        // Для того чтобы не печатать лишние пробелы, проверим размер
        if(glyph.img.sizeInBytes() > 0)
        {
            out << "   ";
        }

        outImage(out, glyph.img, idx);
        // чтобы вывести запятые, кроме последнего изображения
        if(glyph.key != _glyphs->last().key && glyph.img.sizeInBytes() > 0)
        {
            out << ", ";
        }

        out << "\n";
    }
    out << "};\n";
    out << "//  Data size: " << idx << "\n\n";

    //--------------------------- Генерация описания глифов ------------------------------------------------------------
    out << "// glyphs in bitmaps\n";
    out << "static const glyph_t glyphs[] =\n"
           "{\n";
    idx = 0;
    for(auto &glyph: *_glyphs)
    {
        out << "    { ";
        if(_settings->genPointer())
        {
            out << "bitmaps + ";
        }
        out << glyph.idx
            << ", " << glyph.width
            << ", " << glyph.height
            << ", " << glyph.xAdvance
            << ", " << glyph.dx
            << ", " << glyph.dy << " }";
        if(glyph.key != _glyphs->last().key)
        {
            out << ", ";
        }
        else
        {
            out << "  ";
        }
        out << QString("// U+%1   ").arg(glyph.key, 4, 16, QChar('0')) << "'" << QString(QChar(glyph.key)) <<
               "' " << ", [" << idx++<< "]\n";
    }
    out << "};\n\n";

    uint16_t startKey = _glyphs->first().key;
    uint16_t groupLength = 0;
    idx = 0;
    //-------------------------------- Генерация групп глифов ----------------------------------------------------------
    out << "// glyphs ranges\n";
    out << "static const glyph_array_t glyph_ranges[] =\n"
           "{\n";
    for(auto &glyph: *_glyphs)
    {
        if( (glyph.key - (startKey + groupLength)) > 0)
        {
            out << "    { " << Qt::showbase << Qt::hex << startKey<< ", ";
            out <<  Qt::dec << groupLength << ", " <<
                    "glyphs + " << idx << " },\n";
            idx += groupLength;
            groupLength = 0;
            startKey = glyph.key;
        }
        ++groupLength;
    }
    out << "    { " << Qt::showbase << Qt::hex << startKey<< ", ";
    out <<  Qt::dec << groupLength << ", " <<
            "glyphs + " << idx << " },\n";
    out << "    { 0x0, 0, 0 }\n";
    out << "};\n\n";


    xWidthAverage += dXWidth/_glyphs->size();
    //---------------------------- Создание объекта шрифта -------------------------------------------------------------
    if(_settings->genGenFunc())
    {
    out << "dgx_font_t* TerminusTTFMedium12()\n";
    out << "{\n";
    out << "    static dgx_font_t rval = "
           "{"
           " glyph_ranges, " <<
           yAdvance << ", " <<
           yOffsetLowest << ", " <<
           xWidest << ", " <<
           xWidthAverage <<
           "};\n";
    out << "    return &rval;\n";
    out << "}\n\n";
    }
    else
    {
        out << "dgx_font_t TerminusTTFMedium12 = "
               "{ "
               " glyph_ranges, " <<
               yAdvance << ", " <<
               yOffsetLowest << ", " <<
               xWidest << ", " <<
               xWidthAverage <<
               "};\n\n";
    }
    out << _settings->genPostfix() << "\n\n";
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------ P R O T E C T E D ---------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
QDir IOFontGode::filePath(const QString &&dir)
{
    QDir path = _basePath;
    if(_settings->baseGenPathStruct())
    {
        if(path.mkpath(dir))
        {
            path.cd(dir);
        }
    }
    return path;
}

//----------------------------------------------------------------------------------------------------------------------
void IOFontGode::outImage(QTextStream &out, QImage &img, uint16_t &idx)
{
    int numInLine = 0;
    uint8_t byte = 0;
    int x = 0;
    int numBit = 0;

    // Пройдёмся по всем пикселям глифа
    for(int y  = 0; y < img.height(); ++y)
    {
        for(x = 0; x < img.width(); ++x, ++numBit)
        {
//                qDebug() << "(" << x << ", " << y << "): " << glyph.img.pixelIndex(x ,y);
            byte |= !img.pixelIndex(x ,y);
            // Если набрали 8 бит, то выведем их
            if((numBit % 8) == 7)
            {
                outHexByte(out, byte, numInLine);
                ++idx;
                out << ", ";
                numBit = -1;
                byte = 0;
            }
            byte <<=1;
        }

        if(!_settings->genPack())
        {
            if(numBit%8 != 0)
            {
                byte <<= 8 - (x+1)%8;
                outHexByte(out, byte, numInLine);
                ++idx;
                numBit = 0;
            }
            // Чтобы не вывести запятую после вывода картинки
            if(y != img.height() - 1)
            {
                out << ",";
            }
        }
    }
    if(numBit%8 != 0)
    {
        byte <<= 8 - (x+1)%8;
        outHexByte(out, byte, numInLine);
        ++idx;
    }

}

//----------------------------------------------------------------------------------------------------------------------
QTextStream & IOFontGode::outHexByte(QTextStream &out, uint8_t byte, int &numInLine, int Numberlen)
{
    if(numInLine%17 == 16)
    {
        out << "\n   ";
        numInLine = 0;
    }
    ++numInLine;

    return out << QString(" 0x%1").arg(byte, Numberlen, 16, QChar('0'));
}

