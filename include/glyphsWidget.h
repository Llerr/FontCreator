#ifndef GLYPHS_H
#define GLYPHS_H

#include <QWidget>
#include <QMap>


#include "drawGlyphsWidget.h"
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

    friend class DrawGlyphsWidget;
public:
    explicit GlyphsWidget(QWidget *parent = nullptr);
    ~GlyphsWidget();

    void removeGlyph(int key);
    void setGlyph(int key, Glyph glyph);

    GlyphsMap *glyphs() {return &_glyphs;}
    const QString &fontName(){return _fontName;}

    int save(QJsonObject &json);
    int load(QJsonObject &json);
    bool isNeedSave(){return _modifed;}
    void setNeedSave(bool need);
signals:
    void glyphSelected(const Glyph &);
    void neededSave(bool need);

public slots:
    void deletedGlyph();
    void receiveGlyphs(const GlyphsMap &glyphs, const QString &name);
    void editFinish(const Glyph &glyph);

private slots:
//    void on_btnDel_clicked();
//    void on_btnPlus_clicked();
//    void on_btnMinus_clicked();

protected:
        void wheelEvent(QWheelEvent *event) override;

private:
    Ui::GlyphsWidget *_ui; ///< Указатель на форму

    DrawGlyphsWidget *_wgtDraw; ///< Виджет отрисовки гифов

    QString _fontName; ///< Имя шрифта
    GlyphsMap _glyphs; ///< Набор глифов с изображениями

    bool _modifed; ///< Набор глифов изменён и требуется сохранение
};

#endif // GLYPHS_H
