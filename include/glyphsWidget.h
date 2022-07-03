#ifndef GLYPHS_H
#define GLYPHS_H

#include <QWidget>
#include <QMap>

#include "glyph.h"

namespace Ui {
class GlyphsWidget;
}

///
/// \brief class GlyphsWidget отображает выбранные символы (глифы)
///
class GlyphsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GlyphsWidget(QWidget *parent = nullptr);
    ~GlyphsWidget();

    void removeGlyph(int key);
    void setGlyph(int key, Glyph glyph);
private:
    Ui::GlyphsWidget *_ui; ///< Указатель на форму

    QMap<int, Glyph> _glyphs; ///< Набор глифов с изображениями
};

#endif // GLYPHS_H
