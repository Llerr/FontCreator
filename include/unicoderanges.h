#ifndef UNICODERANGES_H
#define UNICODERANGES_H

#include <QString>
#include <QVector>
#include <QVariant>

//----------------------------------------------------------------------------------------------------------------------
struct UnicodeRange
{
    UnicodeRange(int id = -1, int startIn = -1, int endIn = -1, const QString &nameIn = QString()):
        idx(id),
        start(startIn),
        end(endIn),
        name(nameIn)
    {}
    int idx;   ///< Индекс набора
    int start; ///< Начальный код символа в наборе (уникод)
    int end;   ///< конечный код символа в наборе
    QString name; ///< Имя набора
};
 Q_DECLARE_METATYPE(UnicodeRange)

//----------------------------------------------------------------------------------------------------------------------
class UnicodeRanges
{
public:
    UnicodeRanges();

    static UnicodeRange getRange(const QString &name) ;
    static const QVector<UnicodeRange> &getUnicodeRanges();

public:
    static const QVector<UnicodeRange> unicodesRanges; ///< Список групп кодов
};

#endif // UNICODERANGES_H
