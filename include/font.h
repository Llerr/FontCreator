#ifndef FONT_H
#define FONT_H

#include <QWidget>

namespace Ui {
class Font;
}

class Font : public QWidget
{
    Q_OBJECT

public:
    explicit Font(QWidget *parent = nullptr);
    ~Font();

private:
    Ui::Font *ui;
};

#endif // FONT_H
