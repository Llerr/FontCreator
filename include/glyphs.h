#ifndef GLYPHS_H
#define GLYPHS_H

#include <QWidget>

namespace Ui {
class Glyphs;
}

class Glyphs : public QWidget
{
    Q_OBJECT

public:
    explicit Glyphs(QWidget *parent = nullptr);
    ~Glyphs();

private:
    Ui::Glyphs *ui;
};

#endif // GLYPHS_H
