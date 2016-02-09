#include "popupcolor.h"
#include "popupcolorstylesheethelper.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#endif
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

PopUpColor::PopUpColor(QWidget *parent) :
    QWidget(parent),
    m_leftMouseButtonPressed(false)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");

    setLayout(&layout);
    layout.setSpacing(0);

    layout.addWidget(&popUpWidget,0,0);
    popUpWidget.setLayout(&layoutPopUp);
    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layoutPopUp.addWidget(&label,0,0,1,4);
    layoutPopUp.addWidget(&closeButton,0,4);
    layoutPopUp.addWidget(&pickerButton,1,0);
    layoutPopUp.addWidget(&gradationButton,1,1);
    layoutPopUp.addWidget(&copyButton,1,2);
    comboBox.addItems(QStringList() << "HEX" << "RGB" << "CMYK" << "HSV" << "HSL");
    comboBox.setCurrentIndex(0);
    layoutPopUp.addWidget(&comboBox,1,3,1,2);

    layout.addWidget(&sliderWidget,1,0);
    sliderWidget.setLayout(&layoutSlider);
    sliderHue.setOrientation(Qt::Horizontal);
    sliderHue.setRange(0,359);
    layoutSlider.addWidget(&sliderHue,0,1);
    sliderSaturation.setOrientation(Qt::Horizontal);
    sliderSaturation.setRange(0,100);
    layoutSlider.addWidget(&sliderSaturation,1,1);
    sliderLightness.setOrientation(Qt::Horizontal);
    sliderLightness.setRange(0,100);
    layoutSlider.addWidget(&sliderLightness,2,1);
    layoutSlider.addWidget(&imgHue,0,0);
    layoutSlider.addWidget(&imgSaturation,1,0);
    layoutSlider.addWidget(&imgLightness,2,0);
    sliderWidget.setContentsMargins(6,2,6,2);
    imgSaturation.setFixedSize(16,16);
    imgLightness.setFixedSize(16,16);

    layout.addWidget(&gradationWidget,2,0);
    gradationWidget.setLayout(&layoutGradation);
    layoutGradation.setSpacing(0);
    layoutGradation.setContentsMargins(7,0,7,1);
    for(int i=0; i<COUNT_GRADATION; ++i){
        labelGradation[i].setCurrentLightness(0.1*(i+1));
        layoutGradation.addWidget(&labelGradation[i],9-i,0);
    }

    connect(&comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PopUpColor::changeIndexComboBoxColor);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::slotHide);
    connect(&closeButton, &QToolButton::clicked, &dummyTransparentWindow, &TransparentWindow::hide);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::backColor);
    connect(&pickerButton, &QToolButton::clicked, this, &PopUpColor::pickerButtonClicked);
    connect(&gradationButton, &QToolButton::clicked, this, &PopUpColor::gradationButtonClicked);
    connect(&copyButton, &QToolButton::clicked, this, &PopUpColor::copyButtonClicked);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeLabelText);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeStyleSheets);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeSliders);
    connect(&dummyTransparentWindow, &TransparentWindow::changeColor, this, &PopUpColor::setCurrentColor);
    connect(&dummyTransparentWindow, &TransparentWindow::backColor, this, &PopUpColor::backColor);
    connect(&dummyTransparentWindow, &TransparentWindow::saveColor, this, &PopUpColor::saveColor);
    connect(&dummyTransparentWindow, &TransparentWindow::visibleChanged, this, &PopUpColor::updateStyleSheets);
    connect(&sliderHue, &QSlider::valueChanged, this, &PopUpColor::setHue);
    connect(&sliderSaturation, &QSlider::valueChanged, this, &PopUpColor::setSaturation);
    connect(&sliderLightness, &QSlider::valueChanged, this, &PopUpColor::setLightness);
    connect(&sliderHue, &QSlider::sliderPressed, this, &PopUpColor::sliderPress);
    connect(&sliderSaturation, &QSlider::sliderPressed, this, &PopUpColor::sliderPress);
    connect(&sliderLightness, &QSlider::sliderPressed, this, &PopUpColor::sliderPress);
    connect(&sliderHue, &QSlider::sliderReleased, this, &PopUpColor::sliderRelease);
    connect(&sliderSaturation, &QSlider::sliderReleased, this, &PopUpColor::sliderRelease);
    connect(&sliderLightness, &QSlider::sliderReleased, this, &PopUpColor::sliderRelease);

    for(int i=0; i<COUNT_GRADATION; ++i){
        connect(this, &PopUpColor::currentColorChanged, &labelGradation[i], &GradationLabel::setCurrentColor);
        connect(&labelGradation[i], &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
        connect(&labelGradation[i], &GradationLabel::colorForSet, this, &PopUpColor::setCurrentColor);
    }

    reloadSettings();
    setCurrentColor(QColor(Qt::white));
    tempCurrentColor = QColor(Qt::white);

    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    sliderPressed = false;
    adjustSize();
}

void PopUpColor::saveSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(SETTINGS_POS_X,posWin.x());
    settings.setValue(SETTINGS_POS_Y,posWin.y());
}

PopUpColor::~PopUpColor()
{
}

QPoint PopUpColor::previousPosition() const
{
    return m_previousPosition;
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
            if(!dummyTransparentWindow.isVisible()){
                QScreen *screen = QApplication::primaryScreen();
                QImage img = screen->grabWindow(0).toImage();
                QColor color;
                color.setRgb(img.pixel(QCursor::pos()));;
                setCurrentColor(color);
                tempCurrentColor = color;
                gradationWidget.setVisible(false);
                sliderWidget.setVisible(false);
                gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), currentColor));
                adjustSize();
                (followCursor) ? showPos(QCursor::pos()) : showPos(posWin);
                slotCopyBuffer(getCurrentColor());
            }
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

void PopUpColor::slotShow()
{
    showPos(posWin);
}

void PopUpColor::showPos(QPoint point)
{
    adjustSize();
    int x,y;
    if(point == QPoint(0,0)){
        x = QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x();
        y = QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y();
    }else{
        x = ((QApplication::desktop()->width() - point.x()) < width())? point.x() - width() : point.x();
        y = ((QApplication::desktop()->height() - point.y()) < height())? point.y() - height() : point.y();
    }
    setGeometry(x,y,width(),height());

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
    copyBuffer = settings.value(SETTINGS_COPY_BUFF, true).toBool();
    typeCopyBuffer = settings.value(SETTINGS_TYPE_BUFF, 0).toInt();
    followCursor = settings.value(SETTINGS_FOLLOW_CURSOR, true).toBool();
    posWin.setX(settings.value(SETTINGS_POS_X,0).toInt());
    posWin.setY(settings.value(SETTINGS_POS_Y,0).toInt());
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

void PopUpColor::copyButtonClicked()
{
    slotCopyBuffer(currentColor);
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
    changeLabelText(getCurrentColor());
    slotCopyBuffer(getCurrentColor());
}

void PopUpColor::backColor()
{
    setCurrentColor(tempCurrentColor);
}

void PopUpColor::saveColor()
{
    tempCurrentColor = getCurrentColor();
    slotCopyBuffer(getCurrentColor());
}

void PopUpColor::sliderPress()
{
    sliderPressed = true;
}

void PopUpColor::sliderRelease()
{
    sliderPressed = false;
}

void PopUpColor::setHue(int value)
{
    QColor color;
    color.setHslF((qreal)value/359,
                  currentColor.hslSaturationF(),
                  currentColor.lightnessF());
    setCurrentColor(color);
}

void PopUpColor::setSaturation(int value)
{
    QColor color;
    color.setHslF(currentColor.hslHueF(),
                  (qreal)value/100,
                  currentColor.lightnessF());
    setCurrentColor(color);
}

void PopUpColor::setLightness(int value)
{
    QColor color;
    color.setHslF(currentColor.hslHueF(),
                  currentColor.hslSaturationF(),
                  (qreal)value/100);
    setCurrentColor(color);
}

void PopUpColor::changeStyleSheets(const QColor &color)
{
    comboBox.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradationCombobox(color));
    label.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCodeLabel(color));
    pickerButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfPicker(dummyTransparentWindow.isVisible(), color));
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), color));
    copyButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCopy(color));
    closeButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCloseButton(color));
    sliderWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfSliderWidget(color));
    gradationWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradationWidget(color));
    popUpWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfPopUpWidget(color));
    sliderHue.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfSlider(color));
    sliderLightness.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfSlider(color));
    sliderSaturation.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfSlider(color));
    imgHue.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfImageHue(color));
    imgSaturation.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfImageSaturation(color));
    imgLightness.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfImageLightness(color));

    adjustSize();
}

void PopUpColor::changeLabelText(const QColor &color)
{
    switch (typeCopyBuffer) {
    case 0:
        label.setText(color.name());
        break;
    case 1:
        label.setText("RGB:\t" +
                      QString::number(color.red()) + " " +
                      QString::number(color.green()) + " " +
                      QString::number(color.blue()));
        break;
    case 2:
        label.setText("CMYK:\t" +
                      QString::number(color.cyan()) + " " +
                      QString::number(color.magenta()) + " " +
                      QString::number(color.yellow()) + " " +
                      QString::number(color.black()));
        break;
    case 3:
        label.setText("HSV:\t" +
                      QString::number(color.hsvHue()) + " " +
                      QString::number(round(color.hsvSaturationF()*100)) + "% " +
                      QString::number(round(color.valueF()*100)) + "%");
        break;
    case 4:
        label.setText("HSL:\t" +
                      QString::number(color.hslHue()) + " " +
                      QString::number(round(color.hslSaturationF()*100)) + "% " +
                      QString::number(round(color.lightnessF()*100)) + "%");
        break;
    default:
        break;
    }
    adjustSize();
}

void PopUpColor::changeSliders(const QColor &color)
{
    if(!sliderPressed){
        sliderHue.setValue(color.hslHue());
        sliderSaturation.setValue(round(color.hslSaturationF()*100));
        sliderLightness.setValue(round(color.lightnessF()*100));
    }
}

void PopUpColor::slotCopyBuffer(const QColor &color)
{
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
                                               QString::number(round(color.hsvSaturationF()*100)) + " " +
                                               QString::number(round(color.valueF()*100)));
            break;
        case 4: // HSL
            QApplication::clipboard()->setText(QString::number(color.hslHue()) + " " +
                                               QString::number(round(color.hslSaturationF()*100)) + " " +
                                               QString::number(round(color.lightnessF()*100)));
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
    if (popupOpacity == opacity)
        return;

    popupOpacity = opacity;
    setWindowOpacity(opacity);
}

void PopUpColor::setCurrentColor(QColor color)
{
    if (currentColor == color)
        return;

    currentColor = color;
    emit currentColorChanged(color);
}

void PopUpColor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->y() < pickerButton.y()) {
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->pos());
    }
    return QWidget::mousePressEvent(event);
}

void PopUpColor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_leftMouseButtonPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

void PopUpColor::mouseMoveEvent(QMouseEvent *event)
{
    if (m_leftMouseButtonPressed) {
        auto dx = event->x() - m_previousPosition.x();
        auto dy = event->y() - m_previousPosition.y();
        setGeometry(x() + dx, y() + dy, width(), height());
    }
    return QWidget::mouseMoveEvent(event);
}

void PopUpColor::setPreviousPosition(const QPoint &previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged(previousPosition);
}

float PopUpColor::getPopupOpacity() const
{
    return popupOpacity;
}

QColor PopUpColor::getCurrentColor() const
{
    return currentColor;
}

void PopUpColor::slotHide()
{
    posWin = pos();
    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), currentColor));
    adjustSize();
    hide();
}

void PopUpColor::gradationButtonClicked()
{
    gradationWidget.setVisible(!gradationWidget.isVisible());
    sliderWidget.setVisible(!sliderWidget.isVisible());
    updateStyleSheets();

    adjustSize();
}

void PopUpColor::updateStyleSheets()
{
    changeStyleSheets(getCurrentColor());
}
