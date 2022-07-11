#ifndef EDITGLYPHWIDGET_H
#define EDITGLYPHWIDGET_H

#include <QWidget>

#include "glyph.h"

class DrawEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawEditWidget(Glyph &glyph, QWidget *parent = nullptr);

    QSize sizeHint() const override;

//    void on_btnCancel_clicked();
//    void on_btnOk_clicked();
    void on_btnZoomOut_clicked();
    void on_btnZoomIn_clicked();

signals:
protected:
//    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Glyph &_glyph; ///< Глиф для редактирования

    int _scale;
};

#endif // EDITGLYPHWIDGET_H