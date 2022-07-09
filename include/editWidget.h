#ifndef EDIT_H
#define EDIT_H

#include <QWidget>

#include "glyph.h"

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = nullptr);
    ~EditWidget();

public slots:
    void receiveGlyph(int key);
private:
    Ui::EditWidget *ui;

    int _sale;
    Glyph _editGlyph;
};

#endif // EDIT_H
