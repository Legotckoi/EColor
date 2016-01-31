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
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // TRANSPARENTWINDOW_H
