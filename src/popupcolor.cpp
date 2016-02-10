#include "popupcolor.h"
#include "popupcolorstylesheethelper.h"
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
    m_leftMouseButtonPressed(false),
    m_sliderPressed(false)
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

    setCurrentColor(QColor(Qt::white));
    tempCurrentColor = QColor(Qt::white);

    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
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

void PopUpColor::onHotKeyShowPressed()
{
    if(!dummyTransparentWindow.isVisible()){
        QScreen *screen = QApplication::primaryScreen();
        QImage img = screen->grabWindow(0).toImage();
        QColor color;
        color.setRgb(img.pixel(QCursor::pos()));;
        setCurrentColor(color);
        tempCurrentColor = color;
        gradationWidget.setVisible(false);
        sliderWidget.setVisible(false);
        gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), m_currentColor));
        adjustSize();
        (followCursor) ? showPos(QCursor::pos()) : showPos(posWin);
        slotCopyBuffer(m_currentColor);
    }
}

void PopUpColor::onHotKeyPixmapPressed()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    QPixmap pixmap = QApplication::primaryScreen()->grabWindow(0);
    pixmap.save(settings.value(SETTINGS_PATH_SCREENSHOTS, QVariant()).toString() +
                QDateTime::currentDateTime().toString("/Screen_yyyy-MM-dd_hh-mm-ss") +
                ".png", "PNG");
    QApplication::clipboard()->setPixmap(pixmap);
}

void PopUpColor::slotShow()
{
    changeLabelText(m_currentColor);
    changeStyleSheets(m_currentColor);
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
    keys = QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString());
    emit hotKeysSettingsReloading(keys, settings.value(SETTINGS_ALLOW_SCREENSHOTS, false).toBool());
    comboBox.setCurrentIndex(typeCopyBuffer);
}

void PopUpColor::pickerButtonClicked()
{
    tempCurrentColor = m_currentColor;
    dummyTransparentWindow.showFullScreen();
}

void PopUpColor::copyButtonClicked()
{
    slotCopyBuffer(m_currentColor);
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
    changeLabelText(m_currentColor);
    slotCopyBuffer(m_currentColor);
}

void PopUpColor::backColor()
{
    setCurrentColor(tempCurrentColor);
}

void PopUpColor::saveColor()
{
    tempCurrentColor = m_currentColor;
    slotCopyBuffer(m_currentColor);
}

void PopUpColor::sliderPress()
{
    m_sliderPressed = true;
}

void PopUpColor::sliderRelease()
{
    m_sliderPressed = false;
}

void PopUpColor::setHue(int value)
{
    QColor color;
    color.setHslF((qreal)value/359,
                  m_currentColor.hslSaturationF(),
                  m_currentColor.lightnessF());
    setCurrentColor(color);
}

void PopUpColor::setSaturation(int value)
{
    QColor color;
    color.setHslF(m_currentColor.hslHueF(),
                  (qreal)value/100,
                  m_currentColor.lightnessF());
    setCurrentColor(color);
}

void PopUpColor::setLightness(int value)
{
    QColor color;
    color.setHslF(m_currentColor.hslHueF(),
                  m_currentColor.hslSaturationF(),
                  (qreal)value/100);
    setCurrentColor(color);
}

void PopUpColor::updateStyleSheets()
{
    changeStyleSheets(m_currentColor);
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
    if(!m_sliderPressed){
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

void PopUpColor::setPopupOpacity(float opacity)
{
    if (m_popupOpacity == opacity)
        return;

    m_popupOpacity = opacity;
    setWindowOpacity(opacity);
}

void PopUpColor::setCurrentColor(QColor color)
{
    if (m_currentColor == color)
        return;

    m_currentColor = color;
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

float PopUpColor::popupOpacity() const
{
    return m_popupOpacity;
}

QColor PopUpColor::currentColor() const
{
    return m_currentColor;
}

void PopUpColor::slotHide()
{
    posWin = pos();
    if(dummyTransparentWindow.isVisible()) backColor();
    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), m_currentColor));
    adjustSize();
    hide();
}

void PopUpColor::gradationButtonClicked()
{
    gradationWidget.setVisible(!gradationWidget.isVisible());
    sliderWidget.setVisible(!sliderWidget.isVisible());
    changeStyleSheets(m_currentColor);

    adjustSize();
}
