#ifndef EDIT_H
#define EDIT_H

#include <QWidget>

#include "glyph.h"
#include "drawEditWidget.h"

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = nullptr);
    ~EditWidget();

signals:
    void editFinished(const Glyph &glyph);

public slots:
    void receiveGlyph(const Glyph &glyph);

private slots:
    void on_btnCancel_clicked();
    void on_btnOk_clicked();
    void on_btnZoomOut_clicked();
    void on_btnZoomIn_clicked();

    void on_edtUnicode_editingFinished();
    void on_edtXAdvance_editingFinished();
    void on_edtWidth_editingFinished();
    void on_edtHeight_editingFinished();
    void on_edtDX_editingFinished();
    void on_edtDY_editingFinished();

private:
    void updateEditFields();
private:
    Ui::EditWidget *_ui;

    DrawTextWidget *_wgtEdit;

    int _sale;

    Glyph _glyphIn;   ///< Полученный глиф
    Glyph _glyphEdt;  ///< Редактируемый глиф
};

#endif // EDIT_H
