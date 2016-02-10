#ifndef TRANSPARENTWINDOW_H
#define TRANSPARENTWINDOW_H

#include <QWindow>
#include <QMouseEvent>
#include <QKeyEvent>

class TransparentWindow : public QWindow
{
    Q_OBJECT
public:
    explicit TransparentWindow(QWindow *parent = 0);

signals:
    void changeColor(const QColor &color);
    void saveColor();
    void backColor();

public slots:

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // TRANSPARENTWINDOW_H
