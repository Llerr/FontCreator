#include <QJsonObject>
#include <QImage>

#include "glyph.h"
#include "qdebug.h"
#include "qimage.h"
#include "qnamespace.h"
#include "qrgb.h"

//----------------------------------------------------------------------------------------------------------------------
QImage invert(QImage &img)
{
    for(int x = 0; x < img.width(); ++x)
    {
        for(int y = 0; y < img.height(); ++y)
        {
            img.setPixel(x,y, !img.pixelIndex(x,y));
        }
    }
    return img;
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
Glyph::Glyph():
    key(0),
    img(),
    width(0),
    height(0),
    dx(0),
    dy(0),
    xAdvance(0),
    yAdvance(0)
{
    img.convertTo(QImage::Format_Mono);
    img.setColor(1, qRgb(0,0,0));
    img.setColor(0, qRgb(255,255,255));
}

//----------------------------------------------------------------------------------------------------------------------
int Glyph::save(QJsonObject &json)
{
    QString fileName = QString::number(key, 16).rightJustified(4, '0') + ".xpm";

    json["key"] = key;
    json["dX"] = dx;
    json["dY"] = dy;
    json["xAdvance"] = xAdvance;
    json["yAdvance"] = yAdvance;
    json["height"] = height;
    json["width"] = width;
    json["fileName"] = fileName;
    qDebug() << "Img pixel: " << img.pixel(1,1);
    if(img.save(fileName) == false)
    {
        qWarning() << "Error save file: " << fileName;
        return -1;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
int Glyph::load(QJsonObject &json)
{
    int numLoaded = 0;
    if (json.contains("key") && json["key"].isDouble())
    {
        key = json["key"].toInt();
        ++numLoaded;
    }
    if (json.contains("dX") && json["dX"].isDouble())
    {
        dx = json["dX"].toInt();
        ++numLoaded;
    }
    if (json.contains("dY") && json["dY"].isDouble())
    {
        dy = json["dY"].toInt();
        ++numLoaded;
    }
    if (json.contains("xAdvance") && json["xAdvance"].isDouble())
    {
        xAdvance = json["xAdvance"].toInt();
        ++numLoaded;
    }
    if (json.contains("yAdvance") && json["yAdvance"].isDouble())
    {
        yAdvance = json["yAdvance"].toInt();
        ++numLoaded;
    }
    if (json.contains("height") && json["height"].isDouble())
    {
        height = json["height"].toInt();
        ++numLoaded;
    };
    if (json.contains("width") && json["width"].isDouble())
    {
        width = json["width"].toInt();
        ++numLoaded;
    }
    if (json.contains("fileName") && json["fileName"].isString())
    {
        QString fileName = json["fileName"].toString();
//        QImage img(boundRect.size(), QImage::Format_Mono);
//        img.format();
//        QImage tmpImg(fileName);
//        tmpImg.reinterpretAsFormat(QImage::Format_Mono);
//        img = tmpImg.convertToFormat(QImage::Format_Mono);
        img.load(fileName);
        img.convertTo(QImage::Format_Mono);
        ++numLoaded;
    }
    return (numLoaded == 8)?0:-1;
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
QDebug operator<<(QDebug stream, const Glyph &gl)
{
     stream.nospace() << Qt::hex << gl.key << " - symb: " << QString(QChar(gl.key))
                      << Qt::dec << " size (" << gl.width << ", " << gl.height << ")"
                      << ", dx:dy " << gl.dx << ":" << gl.dy
                      << ", advance [" << gl.xAdvance << ", " << gl.yAdvance << "] " << gl.img;
     return stream.maybeSpace();
}

//----------------------------------------------------------------------------------------------------------------------
int getByIndex(GlyphsMap &map, int idx)
{
    if(idx < 0 || idx >= map.size())
        return -1;
    for(auto &&glyp:map)
    {
        if(idx == 0)
        {
            return glyp.key;
        }
        --idx;
    }
    return -1;
}

