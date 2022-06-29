#ifndef EDIT_H
#define EDIT_H

#include <QWidget>

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = nullptr);
    ~EditWidget();

private:
    Ui::EditWidget *ui;
};

#endif // EDIT_H
