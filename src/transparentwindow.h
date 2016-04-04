#ifndef TRANSPARENTWINDOW_H
#define TRANSPARENTWINDOW_H

#include <QWindow>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

class TransparentWindow : public QWindow
{
    Q_OBJECT
public:
    explicit TransparentWindow(QWindow *parent = 0);

signals:
    void changeColor(const QColor &color);
    void saveColor();
    void backColor();
    void cursorX(QString x);
    void cursorY(QString y);
    void loupeImage(QPixmap &pixmap);

public slots:

private:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // TRANSPARENTWINDOW_H
