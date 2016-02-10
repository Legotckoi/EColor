#include "transparentwindow.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

TransparentWindow::TransparentWindow(QWindow *parent) : QWindow(parent)
{
    QPixmap imagePipette(":/images/eyedropper-black.png");
    QPixmap pipette = imagePipette.scaled(24,24,Qt::KeepAspectRatio);
    QCursor cursor = QCursor(pipette,4,20);
    setCursor(cursor);
}

void TransparentWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
    QImage img = QApplication::primaryScreen()->grabWindow(0,
                                                           QCursor::pos().x(),
                                                           QCursor::pos().y(),
                                                           1,
                                                           1).toImage();
    QColor color;
    color.setRgb(img.pixel(0,0));
    emit changeColor(color);
}

void TransparentWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit saveColor();
    } else {
        emit backColor();
    }
    hide();
}

void TransparentWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        emit backColor();
        hide();
    }
}

