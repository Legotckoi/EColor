#include "codelabel.h"
#include <QDebug>

CodeLabel::CodeLabel(QWidget *parent) : QLabel(parent)
{

}

void CodeLabel::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QPoint pos;
    pos.setX(QCursor::pos().x() - posOfWidget.x() - 11);
    pos.setY(QCursor::pos().y() - posOfWidget.y() - 11);
    emit setPos(pos);
}

void CodeLabel::mousePressEvent(QMouseEvent *event)
{
    posOfWidget = event->pos();
}

