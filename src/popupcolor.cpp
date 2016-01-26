#include "popupcolor.h"
#include "settings.h"
#include <windows.h>
#include <QSettings>
#include <QDir>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QScreen>
#include <QClipboard>
#include <QDateTime>

PopUpColor::PopUpColor(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &PopUpColor::hide);
    setLayout(&layout);

    groupBox.setStyleSheet("QGroupBox { "
                           "border: 0px;"
                           "padding: 0px;"
                           "margin: 0px; }");
    layout.addWidget(&groupBox,1,1,2,1);
    groupBox.setLayout(&vBox);
    QPalette palette;
    palette.setColor(QPalette::Foreground,Qt::white);
    rHex.setPalette(palette);
    rRGB.setPalette(palette);
    rCMYK.setPalette(palette);
    rHSV.setPalette(palette);
    rHex.setText("HEX:\t");
    rRGB.setText("RGB:\t");
    rCMYK.setText("CMYK:\t");
    rHSV.setText("HSV:\t");
    vBox.addWidget(&rHex);
    vBox.addWidget(&rRGB);
    vBox.addWidget(&rCMYK);
    vBox.addWidget(&rHSV);

    colorLabel.setStyleSheet("QLabel {"
                             "margin: 6px;"
                             "width: 32px;"
                             "height: 32px; "
                             "border: 1px solid grey; }");
    layout.addWidget(&colorLabel,0,0,2,1,Qt::AlignTop);

    dropperButton.setIcon(QIcon(":/images/eyedropper.png"));
    dropperButton.setStyleSheet("QToolButton { margin: 6px;"
                                "height: 30px;"
                                "width: 30px; "
                                "border: 1px solid grey; "
                                "background-color: transparent;}"
                                "QToolButton:pressed {"
                                "background-color: grey;}");
    layout.addWidget(&dropperButton,2,0);
    connect(&dropperButton, &QAbstractButton::clicked, this, &PopUpColor::dropperbuttonClicked);

    label.setStyleSheet("QLabel { color: white;"
                        "margin-top: 6px;"
                        "margin-right: 6px; "
                        "margin-left: 6px; "
                        "margin-bottom: 0px;}");
    label.setText(trUtf8("Цвет пикселя"));
    layout.addWidget(&label,0,1);

    reloadSettings();

    setPopupColor(QColor(Qt::white));

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopUpColor::hideAnimation);
}

void PopUpColor::paintEvent(QPaintEvent *event)
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

bool PopUpColor::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    MSG* msg = reinterpret_cast<MSG*>(message);

    if(msg->message == WM_HOTKEY){

        switch (msg->wParam) {
        case 100: {
            QScreen *screen = QApplication::primaryScreen();
            QImage img = screen->grabWindow(0).toImage();
            QRgb b = img.pixel(QCursor::pos());
            QColor c;
            c.setRgb(b);
            setColor(c);
            return true;
            break;
        }
        case 101: {
            QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
            QPixmap pixmap = QApplication::primaryScreen()->grabWindow(0);
            pixmap.save(settings.value(SETTINGS_PATH_SCREENSHOTS, QVariant()).toString() +
                        QDateTime::currentDateTime().toString("/Screen_yyyy-MM-dd_hh-mm-ss") +
                        ".png", "PNG");
            QApplication::clipboard()->setPixmap(pixmap);
            return true;
            break;
        }
        default:
            break;
        }
    }
    return false;
}

void PopUpColor::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    timer->stop();
}

void PopUpColor::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    timer->start(3000);
}

void PopUpColor::setPopupColor(const QColor &color)
{
    currentColor = color;
    QImage image(34, 34, QImage::Format_RGB32);
    QRgb value = color.rgb();
    for(int i = 0; i < 34; i++){
        for(int j = 0; j < 34; j++){
            image.setPixel(i, j, value);
        }
    }

    QPixmap pic;
    pic.convertFromImage(image);
    colorLabel.setPixmap(pic);
    rHex.setText("HEX:\t" + color.name());
    rRGB.setText("RGB:\t" +
                 QString::number(color.red()) + " " +
                 QString::number(color.green()) + " " +
                 QString::number(color.blue()));
    rCMYK.setText("CMYK:\t" +
                  QString::number(color.cyan()) + " " +
                  QString::number(color.magenta()) + " " +
                  QString::number(color.yellow()) + " " +
                  QString::number(color.black()));
    rHSV.setText("HSV:\t" +
                 QString::number(color.hue()) + " " +
                 QString::number(color.saturation()) + " " +
                 QString::number(color.value()));
}

void PopUpColor::show()
{
    adjustSize();
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

void PopUpColor::showPos(QPoint point)
{
    adjustSize();
    int cursorX = (QApplication::desktop()->width() - point.x() < width()) ? (point.x() - width()) : point.x();
    int cursorY = (QApplication::desktop()->height() - point.y() < height()) ? (point.y() - height()) : point.y();

    setGeometry(cursorX, cursorY, width(), height());

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

void PopUpColor::reloadSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    copyBuffer = settings.value(SETTINGS_COPY_BUFF, false).toBool();
    typeCopyBuffer = settings.value(SETTINGS_TYPE_BUFF, 0).toInt();
    followCursor = settings.value(SETTINGS_FOLLOW_CURSOR, false).toBool();
    UnregisterHotKey((HWND) PopUpColor::winId(), 101);
    if(settings.value(SETTINGS_ALLOW_SCREENSHOTS, false).toBool()){
        RegisterHotKey((HWND)PopUpColor::winId(), 101, 0, VK_SNAPSHOT);
    }
    keys = QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString());
    UnregisterHotKey((HWND)PopUpColor::winId(), 100);
    RegisterHotKey((HWND)PopUpColor::winId(), 100, winKeyModificator(keys), winHotKey(keys));
    switch(typeCopyBuffer){
    case 0:
        rHex.setChecked(true);
        break;
    case 1:
        rRGB.setChecked(true);
        break;
    case 2:
        rCMYK.setChecked(true);
        break;
    case 3:
        rHSV.setChecked(true);
        break;
    }
}

void PopUpColor::dropperbuttonClicked()
{

}

void PopUpColor::hideAnimation()
{
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUpColor::hide()
{
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void PopUpColor::setColor(const QColor &color)
{
    setPopupColor(color);
    (followCursor) ? showPos(QCursor::pos()) : show();

    if(copyBuffer){
        switch (typeCopyBuffer) {
        case 0: // HEX
            QApplication::clipboard()->setText(color.name());
            break;
        case 1: // RGB
            QApplication::clipboard()->setText(QString::number(color.red()) + " " +
                                               QString::number(color.green()) + " " +
                                               QString::number(color.blue()));
            break;
        case 2: // CMYK
            QApplication::clipboard()->setText(QString::number(color.cyan()) + " " +
                                               QString::number(color.magenta()) + " " +
                                               QString::number(color.yellow()) + " " +
                                               QString::number(color.black()));
            break;
        case 3: // HSV
            QApplication::clipboard()->setText(QString::number(color.hue()) + " " +
                                               QString::number(color.saturation()) + " " +
                                               QString::number(color.value()));
            break;
        default:
            break;
        }
    }
}

unsigned int PopUpColor::winKeyModificator(QKeySequence sequence)
{
    QStringList list = sequence.toString().split("+");
    unsigned int keyModificator = 0;

    foreach (QString str, list) {
        if(str == "Ctrl"){
            keyModificator += MOD_CONTROL;
            continue;
        } else if(str == "Alt"){
            keyModificator += MOD_ALT;
            continue;
        } else if(str == "Shift"){
            keyModificator += MOD_SHIFT;
            continue;
        }
    }
    return keyModificator;
}

char PopUpColor::winHotKey(QKeySequence sequence)
{
    QStringList list = sequence.toString().split("+");
    char hotKey = 'E';

    foreach (QString str, list) {
        if(str != "Ctrl" && str != "Alt" && str != "Shift"){
            hotKey = str.at(0).unicode();
        }
    }
    return hotKey;
}

void PopUpColor::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;
    setWindowOpacity(opacity);
}

float PopUpColor::getPopupOpacity() const
{
    return popupOpacity;
}
