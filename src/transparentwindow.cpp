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
    auto cursorPos = QCursor::pos();
    QImage img = QApplication::primaryScreen()->grabWindow(0,cursorPos.x(), cursorPos.y(),1,1).toImage();
    QColor color;
    color.setRgb(img.pixel(0,0));
    QImage imgPixmap = QApplication::primaryScreen()->grabWindow(0,cursorPos.x()-12, cursorPos.y()-12,24,24).toImage();
    imgPixmap = imgPixmap.scaled(72,72);
    imgPixmap.setPixelColor(36,36,QColor(Qt::red));
    imgPixmap.setPixelColor(36,37,QColor(Qt::red));
    imgPixmap.setPixelColor(37,36,QColor(Qt::red));
    imgPixmap.setPixelColor(37,37,QColor(Qt::red));
    QPixmap pixmap = QPixmap::fromImage(imgPixmap);
    emit loupeImage(pixmap);
    emit cursorX(QString::number(cursorPos.x()));
    emit cursorY(QString::number(cursorPos.y()));
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

void TransparentWindow::wheelEvent(QWheelEvent *event)
{
    event->accept();
    QImage img = QApplication::primaryScreen()->grabWindow(0,QCursor::pos().x(), QCursor::pos().y(),1,1).toImage();

}

