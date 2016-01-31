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
#include <QPolygonF>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <cmath>
#include "popupmessage.h"

PopUpColor::PopUpColor(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");

    setLayout(&layout);

    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout.addWidget(&label,0,0,1,2);
    layout.addWidget(&closeButton,0,2);
    layout.addWidget(&pickerButton,1,0);
    comboBox.addItems(QStringList() << "HEX" << "RGB" << "CMYK" << "HSV" << "HSL");
    comboBox.setCurrentIndex(0);
    layout.addWidget(&comboBox,1,1,1,2);

    connect(&comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PopUpColor::changeIndexComboBoxColor);
    connect(&label, &CodeLabel::setPos, this, &PopUpColor::showPos);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::hide);
    connect(&closeButton, &QToolButton::clicked, &dummyTransparentWindow, &TransparentWindow::hide);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::backColor);
    connect(&pickerButton, &QToolButton::clicked, this, &PopUpColor::pickerButtonClicked);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeLabelText);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeStyleSheets);
    connect(&dummyTransparentWindow, &TransparentWindow::changeColor, this, &PopUpColor::setCurrentColor);
    connect(&dummyTransparentWindow, &TransparentWindow::backColor, this, &PopUpColor::backColor);
    connect(&dummyTransparentWindow, &TransparentWindow::saveColor, this, &PopUpColor::slotCopyBuffer);

    reloadSettings();
    setCurrentColor(QColor(Qt::white));
}

PopUpColor::~PopUpColor()
{

}

void PopUpColor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(currentColor));
    QColor penColor;
    qreal lightness = currentColor.lightnessF();

    if(lightness < 0.5){
        lightness += 0.09;
    } else {
        lightness -= 0.09;
    }
    penColor.setHslF(currentColor.hslHueF(),
                     currentColor.hslSaturationF(),
                     lightness);

    QPen pen(QBrush(penColor),1);
    painter.setPen(pen);

    QRectF roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 14.5);

    painter.drawRoundedRect(roundedRect, 2, 2);

   /* QPolygonF triangle;
    triangle << QPoint(rect().x() + rect().width() / 2 - 10, rect().y() + rect().height() - 10)
             << QPoint(rect().x() + rect().width() / 2, rect().y() + rect().height())
             << QPoint(rect().x() + rect().width() / 2 + 10, rect().y() + rect().height() - 10);

    QPainterPath trianglePath;
    trianglePath.addPolygon(triangle);

    painter.drawPath(trianglePath);*/
}

bool PopUpColor::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
#ifdef Q_OS_WIN32
    MSG* msg = reinterpret_cast<MSG*>(message);

    if(msg->message == WM_HOTKEY){

        switch (msg->wParam) {
        case 100: {
            QScreen *screen = QApplication::primaryScreen();
            QImage img = screen->grabWindow(0).toImage();
            QColor color;
            color.setRgb(img.pixel(QCursor::pos()));;
            setCurrentColor(color);
            (followCursor) ? showPos(QCursor::pos()) : show();
            slotCopyBuffer();
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
#endif
    return false;
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
    } else {
        animation.setDuration(windowOpacity()*150);
        animation.setStartValue(windowOpacity());
        animation.setEndValue(1.0);
        animation.start();
    }
}

void PopUpColor::showPos(QPoint point)
{
    adjustSize();
    setGeometry(point.x(), point.y(), width(), height());

    if(!isVisible()){
        setWindowOpacity(0.0);

        animation.setDuration(150);
        animation.setStartValue(0.0);
        animation.setEndValue(1.0);

        QWidget::show();

        animation.start();
    } else {
        animation.setDuration(windowOpacity()*150);
        animation.setStartValue(windowOpacity());
        animation.setEndValue(1.0);
        animation.start();
    }
}

void PopUpColor::reloadSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    copyBuffer = settings.value(SETTINGS_COPY_BUFF, false).toBool();
    typeCopyBuffer = settings.value(SETTINGS_TYPE_BUFF, 0).toInt();
    followCursor = settings.value(SETTINGS_FOLLOW_CURSOR, false).toBool();
#ifdef Q_OS_WIN32
    UnregisterHotKey((HWND) PopUpColor::winId(), 101);
    if(settings.value(SETTINGS_ALLOW_SCREENSHOTS, false).toBool()){
        RegisterHotKey((HWND)PopUpColor::winId(), 101, 0, VK_SNAPSHOT);
    }
#endif
    keys = QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString());
#ifdef Q_OS_WIN32
    UnregisterHotKey((HWND)PopUpColor::winId(), 100);
    RegisterHotKey((HWND)PopUpColor::winId(), 100, winKeyModificator(keys), winHotKey(keys));
#endif
    comboBox.setCurrentIndex(typeCopyBuffer);
}

void PopUpColor::pickerButtonClicked()
{
    tempCurrentColor = getCurrentColor();
    dummyTransparentWindow.showFullScreen();
}

void PopUpColor::hideAnimation()
{
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUpColor::changeIndexComboBoxColor(int index)
{
    typeCopyBuffer = index;
    changeLabelText();
    slotCopyBuffer();
    this->repaint();
}

void PopUpColor::backColor()
{
    setCurrentColor(tempCurrentColor);
}

void PopUpColor::setColor(const QColor &color)
{
    setCurrentColor(color);
    (followCursor) ? showPos(QCursor::pos()) : show();
    slotCopyBuffer();
}

void PopUpColor::changeStyleSheets()
{
    QColor color;
    qreal lightness = currentColor.lightnessF();
    if(lightness < 0.5){
        lightness += 0.09;
    } else {
        lightness -= 0.09;
    }

    color.setHslF(currentColor.hslHueF(),
                  currentColor.hslSaturationF(),
                  lightness);
    QString strColor = color.name();
    QString fontColor = (currentColor.lightnessF() > 0.7) ? "#000000" : "#ffffff";
    comboBox.setStyleSheet("QComboBox { color: " + fontColor + "; background-color: " + strColor + "; "
                           "border: none; border-radius: 2px;"
                           "margin-top: 6px;"
                           "margin-right: 6px; "
                           "margin-left: 6px; "
                           "margin-bottom: 8px;"
                           "padding: 6px;"
                           "font-size: 14px; }"
                           "QComboBox::drop-down {border: none;} "
                           "QComboBox::down-arrow {image: url(noimg); border: none;}"
                           "QComboBox QAbstractItemView { "
                           "border: none;"
                           "color: " + fontColor + "; "
                           "background-color: " + strColor + "; "
                           "border-radius: 2px;"
                           "padding: 6px;"
                           "selection-color: " + fontColor + ";"
                           "selection-background-color: " + currentColor.name() + ";}");
    if(currentColor.lightnessF() > 0.7){
        label.setStyleSheet("QLabel { color: black;"
                            "margin-top: 4px;"
                            "margin-right: 6px; "
                            "margin-left: 6px; "
                            "margin-bottom: 0px;"
                            "font-size: 16px; }");
        pickerButton.setStyleSheet("QToolButton { image: url(:/images/eyedropper-black.png);"
                                   "icon-size: 16px;"
                                   "height: 16px;"
                                   "width: 16px;"
                                   "margin: 6px;"
                                   "padding: 6px;"
                                   "border: none;"
                                   "border-radius: 2px;"
                                   "background-color: " + strColor + "; }"
                                   "QToolButton:pressed { background-color: transparent; }");
        closeButton.setStyleSheet("QToolButton { image: url(:/images/close-circle-outline-black.png);"
                                  "icon-size: 16px;"
                                  "height: 16px;"
                                  "width: 16px;"
                                  "margin-top: 4px;"
                                  "margin-right: 6px; "
                                  "margin-left: 6px; "
                                  "margin-bottom: 0px;"
                                  "border: none;}"
                                  "QToolButton:pressed { image: url(:/images/close-circle-black.png);}");
    } else {
        label.setStyleSheet("QLabel { color: white;"
                            "margin-top: 4px;"
                            "margin-right: 6px; "
                            "margin-left: 6px; "
                            "margin-bottom: 0px;"
                            "font-size: 16px; }");
        pickerButton.setStyleSheet("QToolButton { image: url(:/images/eyedropper.png);"
                                   "icon-size: 16px;"
                                   "height: 16px;"
                                   "width: 16px;"
                                   "margin: 6px;"
                                   "padding: 6px;"
                                   "border: none;"
                                   "border-radius: 2px;"
                                   "background-color: " + strColor + "; }"
                                   "QToolButton:pressed { background-color: transparent; }");
        closeButton.setStyleSheet("QToolButton { image: url(:/images/close-circle-outline.png);"
                                  "icon-size: 16px;"
                                  "height: 16px;"
                                  "width: 16px;"
                                  "margin-top: 4px;"
                                  "margin-right: 6px; "
                                  "margin-left: 6px; "
                                  "margin-bottom: 0px;"
                                  "border: none;}"
                                  "QToolButton:pressed { image: url(:/images/close-circle.png);}");
    }
    this->repaint();
}

void PopUpColor::changeLabelText()
{
    switch (typeCopyBuffer) {
    case 0:
        label.setText(currentColor.name());
        break;
    case 1:
        label.setText("RGB:\t" +
                      QString::number(currentColor.red()) + " " +
                      QString::number(currentColor.green()) + " " +
                      QString::number(currentColor.blue()));
        break;
    case 2:
        label.setText("CMYK:\t" +
                      QString::number(currentColor.cyan()) + " " +
                      QString::number(currentColor.magenta()) + " " +
                      QString::number(currentColor.yellow()) + " " +
                      QString::number(currentColor.black()));
        break;
    case 3:
        label.setText("HSV:\t" +
                      QString::number(currentColor.hsvHue()) + " " +
                      QString::number(round(currentColor.hsvSaturationF()*100)) + "% " +
                      QString::number(round(currentColor.valueF()*100)) + "%");
        break;
    case 4:
        label.setText("HSL:\t" +
                      QString::number(currentColor.hslHue()) + " " +
                      QString::number(round(currentColor.hslSaturationF()*100)) + "% " +
                      QString::number(round(currentColor.lightnessF()*100)) + "%");
        break;
    default:
        break;
    }
    adjustSize();
}

void PopUpColor::slotCopyBuffer()
{
    if(copyBuffer){
        switch (typeCopyBuffer) {
        case 0: // HEX
            QApplication::clipboard()->setText(currentColor.name());
            break;
        case 1: // RGB
            QApplication::clipboard()->setText(QString::number(currentColor.red()) + " " +
                                               QString::number(currentColor.green()) + " " +
                                               QString::number(currentColor.blue()));
            break;
        case 2: // CMYK
            QApplication::clipboard()->setText(QString::number(currentColor.cyan()) + " " +
                                               QString::number(currentColor.magenta()) + " " +
                                               QString::number(currentColor.yellow()) + " " +
                                               QString::number(currentColor.black()));
            break;
        case 3: // HSV
            QApplication::clipboard()->setText(QString::number(currentColor.hue()) + " " +
                                               QString::number(round(currentColor.hsvSaturationF()*100)) + " " +
                                               QString::number(round(currentColor.valueF()*100)));
            break;
        case 4: // HSL
            QApplication::clipboard()->setText(QString::number(currentColor.hslHue()) + " " +
                                               QString::number(round(currentColor.hslSaturationF()*100)) + " " +
                                               QString::number(round(currentColor.lightnessF()*100)));
            break;
        default:
            break;
        }
        PopUpMessage::information(this, "Скопировано в буфер обмена");
    }
}

#ifdef Q_OS_WIN32
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
#endif

void PopUpColor::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;
    setWindowOpacity(opacity);
}

void PopUpColor::setCurrentColor(QColor color)
{
    currentColor = color;
    emit currentColorChanged();
}

float PopUpColor::getPopupOpacity() const
{
    return popupOpacity;
}

QColor PopUpColor::getCurrentColor() const
{
    return currentColor;
}
