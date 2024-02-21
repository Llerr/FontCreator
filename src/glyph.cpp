#include <QJsonObject>
#include <QImage>
#include <QDebug>
#include <QPoint>
#include <QFile>
#include <QDataStream>

#include "glyph.h"

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
}

//----------------------------------------------------------------------------------------------------------------------
void Glyph::calcPoints()
{
    points.clear();
    points.reserve(img.width() * img.height());
    for(int y = 0; y < img.height(); ++y)
    {
        for(int x = 0; x < img.width(); ++x)
        {
            int color = img.pixelIndex(x, y);
            if(color > 0)
            {
                points.push_back(QPoint(x,y));
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
int Glyph::save(QJsonObject &json)
{
    QString fileNameImg = QString::number(key, 16).rightJustified(4, '0') + ".xbm";
    QString fileNamePts = QString::number(key, 16).rightJustified(4, '0') + ".vec";

    json["key"] = key;
    json["dX"] = dx;
    json["dY"] = dy;
    json["xAdvance"] = xAdvance;
    json["yAdvance"] = yAdvance;
    json["height"] = height;
    json["width"] = width;
    json["baseLine"] = baseLine;

    json["fileName"] = fileNameImg;
    json["fileNamePts"] = fileNamePts;
    qDebug() << "Key: " << key << "Img pixel (0, 0): " << img.pixelIndex(0,0) << ", fmt: " << img.format();
    if(img.save(fileNameImg) == false)
    {
        qWarning() << "Error save file: " << fileNameImg;
        return -1;
    }
    QFile filePts(fileNamePts);
    filePts.open(QIODevice::WriteOnly);
    if(!filePts.isOpen())
    {
        qWarning() << "Can`t save points sequence";
        return 0;
    }
    QDataStream stream(&filePts);
    stream << points;
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
    if (json.contains("baseLine") && json["baseLine"].isDouble())
    {
        baseLine = json["baseLine"].toInt();
        ++numLoaded;
    }
    if (json.contains("fileName") && json["fileName"].isString())
    {
        QString fileName = json["fileName"].toString();
        img.load(fileName);
        img.convertTo(QImage::Format_Mono);
        qDebug() << "Key: " << key << "Img pixel (0, 0): " << img.pixelIndex(0,0) << ", fmt: " << img.format();
        ++numLoaded;
    }
    if (json.contains("fileNamePts") && json["fileNamePts"].isString())
    {
        QString fileNamePts = json["fileNamePts"].toString();
        ++numLoaded;
        QFile filePts(fileNamePts);
        filePts.open(QIODevice::ReadOnly);
        if(!filePts.isOpen())
        {
            qWarning() << "Can`t load points sequence";
            return 0;
        }
        QDataStream stream(&filePts);
        stream >> points;
    }
    return (numLoaded == 10)?0:-1;
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

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
