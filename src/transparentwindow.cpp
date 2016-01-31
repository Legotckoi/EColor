#include "transparentwindow.h"
#include <QDebug>
#include <QApplication>
#include <QScreen>

TransparentWindow::TransparentWindow(QWindow *parent) : QWindow(parent)
{
    setFlags(Qt::Tool | Qt::FramelessWindowHint);
    QPixmap imagePipette(":/images/eyedropper-black.png");
    QPixmap pipette = imagePipette.scaled(24,24,Qt::KeepAspectRatio);
    QCursor cursor = QCursor(pipette,4,20);
    setCursor(cursor);
}

void TransparentWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    QImage img = QApplication::primaryScreen()->grabWindow(0).toImage();
    QColor color;
    color.setRgb(img.pixel(QCursor::pos()));
    emit changeColor(color);
}

void TransparentWindow::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    if(event->button() == Qt::LeftButton){
        emit saveColor();
    } else {
        emit backColor();
    }
    hide();
}

void TransparentWindow::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    if(event->key() == Qt::Key_Escape) {
        emit backColor();
        hide();
    }
}

