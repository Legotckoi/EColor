#include "popupmessage.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QVBoxLayout>

static void showNewMessage(QWidget *parent, const QString text)
{
    PopUpMessage *msg = new PopUpMessage(parent);
    msg->setPopupText(text);
    msg->show();
}

PopUpMessage::PopUpMessage(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &PopUpMessage::hide);

    setLayout(&layout);

    label.setStyleSheet("QLabel { color : white; "
                        "margin-top: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px;"
                        "margin-bottom: 6px; }");
    layout.addWidget(&label,0,0);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopUpMessage::hideAnimation);
}

PopUpMessage::~PopUpMessage()
{
    delete timer;
}

void PopUpMessage::information(QWidget *parent, const QString text)
{
    showNewMessage(parent, text);
}

void PopUpMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopUpMessage::setPopupText(const QString &text)
{
    label.setText(text);
    adjustSize();
}

void PopUpMessage::show()
{
    setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x(),
                QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                width(),
                height());
    if(!isVisible()){
        setWindowOpacity(0.0);

        animation.setDuration(150);
        animation.setStartValue(0.0);
        animation.setEndValue(1.0);

        QWidget::show();

        animation.start();
        timer->start(3000);
    } else {
        animation.setDuration(windowOpacity()*150);
        animation.setStartValue(windowOpacity());
        animation.setEndValue(1.0);
        animation.start();
        timer->start(3000);
    }
}

void PopUpMessage::hideAnimation()
{
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUpMessage::hide()
{
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
        this->deleteLater();
    }
}

void PopUpMessage::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;
    setWindowOpacity(opacity);
}

float PopUpMessage::getPopupOpacity() const
{
    return popupOpacity;
}
