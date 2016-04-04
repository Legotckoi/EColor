#include "popupcolor.h"
#include "popupcolorstylesheethelper.h"
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QScreen>
#include <QClipboard>
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
    layoutPopUp.addWidget(&label,0,0,1,5);
    layoutPopUp.addWidget(&closeButton,0,5);
    layoutPopUp.addWidget(&labelX,1,0);
    layoutPopUp.addWidget(&lineEditX,1,1,1,2);
    layoutPopUp.addWidget(&labelY,1,3);
    layoutPopUp.addWidget(&lineEditY,1,4,1,2);
    auto coordMaxX = QApplication::desktop()->geometry().width();
    auto coordMaxY = QApplication::desktop()->geometry().height();
    lineEditX.setValidator(new QIntValidator(0,coordMaxX,this));
    lineEditY.setValidator(new QIntValidator(0,coordMaxY,this));
    lineEditX.setText("0");
    lineEditY.setText("0");
    layoutPopUp.addWidget(&pickerButton,2,0);
    layoutPopUp.addWidget(&loupeButton,2,1);
    layoutPopUp.addWidget(&gradationButton,2,2);
    layoutPopUp.addWidget(&copyButton,2,3);
    comboBox.addItems(QStringList() << "HEX" << "RGB" << "CMYK" << "HSV" << "HSL");
    comboBox.setCurrentIndex(0);
    layoutPopUp.addWidget(&comboBox,2,4,1,2);

    layout.addWidget(&loupeWidget,0,1);
    loupeWidget.setLayout(&loupeLayout);
    loupeLayout.addWidget(&loupeLabel,0,0);
    loupeLabel.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfLoupeLabel());
    loupeLayout.setSpacing(6);

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

    label.setToolTip(trUtf8("Текущее значение цвета"));
    QString strCoord = trUtf8("Координата положения захваченного пикселя.\n") +
                       trUtf8("X: от 0 до ") + QString::number(coordMaxX) + "\n" +
                       trUtf8("Y: от 0 до ") + QString::number(coordMaxY);
    labelX.setText(" X: ");
    labelY.setText(" Y: ");
    labelX.setToolTip(strCoord);
    labelY.setToolTip(strCoord);
    lineEditX.setToolTip(strCoord);
    lineEditY.setToolTip(strCoord);
    pickerButton.setToolTip(trUtf8("Пипетка для захвата пикселя"));
    loupeButton.setToolTip(trUtf8("Лупа с областью захвата пикселя\nвокруг пипетки"));
    gradationButton.setToolTip(trUtf8("Корректировка цвета"));
    copyButton.setToolTip(trUtf8("Копировать цвет в буфер обмена"));
    comboBox.setToolTip(trUtf8("Выбор кодировки цвета"));
    imgHue.setToolTip(trUtf8("Тон"));
    imgSaturation.setToolTip(trUtf8("Насыщенность"));
    imgLightness.setToolTip(trUtf8("Яркость"));
    sliderHue.setToolTip(trUtf8("Тон"));
    sliderSaturation.setToolTip(trUtf8("Насыщенность"));
    sliderLightness.setToolTip(trUtf8("Яркость"));

    connect(&comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PopUpColor::changeIndexComboBoxColor);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::slotHide);
    connect(&closeButton, &QToolButton::clicked, &dummyTransparentWindow, &TransparentWindow::hide);
    connect(&pickerButton, &QToolButton::clicked, [=](){tempCurrentColor = m_currentColor;
                                                        tempX = lineEditX.text().toInt();
                                                        tempY = lineEditY.text().toInt();
                                                        dummyTransparentWindow.showFullScreen();});
    connect(&loupeButton, &QToolButton::clicked, [=](){loupeWidget.setVisible(!loupeWidget.isVisible());
                                                       changeStyleSheets(m_currentColor);
                                                       adjustSize();});
    connect(&gradationButton, &QToolButton::clicked, this, &PopUpColor::gradationButtonClicked);
    connect(&copyButton, &QToolButton::clicked, [=](){slotCopyBuffer(m_currentColor);});
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeLabelText);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeStyleSheets);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeSliders);
    connect(&dummyTransparentWindow, &TransparentWindow::changeColor, this, &PopUpColor::setCurrentColor);
    connect(&dummyTransparentWindow, &TransparentWindow::cursorX, &lineEditX, &QLineEdit::setText);
    connect(&dummyTransparentWindow, &TransparentWindow::cursorY, &lineEditY, &QLineEdit::setText);
    connect(&dummyTransparentWindow, &TransparentWindow::backColor, [=](){setCurrentColor(tempCurrentColor);
                                                                          lineEditX.setText(QString::number(tempX));
                                                                          lineEditY.setText(QString::number(tempY));
                                                                          coordinatePressed();});
    connect(&dummyTransparentWindow, &TransparentWindow::saveColor, [=](){tempCurrentColor = m_currentColor;
                                                                          tempX = lineEditX.text().toInt();
                                                                          tempY = lineEditY.text().toInt();
                                                                          slotCopyBuffer(m_currentColor);});
    connect(&dummyTransparentWindow, &TransparentWindow::visibleChanged, [=](){changeStyleSheets(m_currentColor);});
    connect(&dummyTransparentWindow, &TransparentWindow::loupeImage, &loupeLabel, &QLabel::setPixmap);
    connect(&sliderHue, &QSlider::valueChanged, this, &PopUpColor::setHue);
    connect(&sliderSaturation, &QSlider::valueChanged, this, &PopUpColor::setSaturation);
    connect(&sliderLightness, &QSlider::valueChanged, this, &PopUpColor::setLightness);
    connect(&lineEditX, &QLineEdit::returnPressed, this, &PopUpColor::coordinatePressed);
    connect(&lineEditY, &QLineEdit::returnPressed, this, &PopUpColor::coordinatePressed);

    for(int i=0; i<COUNT_GRADATION; ++i){
        connect(this, &PopUpColor::currentColorChanged, &labelGradation[i], &GradationLabel::setCurrentColor);
        connect(&labelGradation[i], &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
        connect(&labelGradation[i], &GradationLabel::colorForSet, this, &PopUpColor::setCurrentColor);
        labelGradation[i].setToolTip("Яркость цвета " + QString::number(10*(i + 1)) + "%\n"
                                     "ЛКМ - копировать в буфер обмена\n"
                                     "ПКМ - выбрать цвет");
    }

    connect(&gShortcutShow, &QGlobalShortcut::activated, this, &PopUpColor::onHotKeyShowPressed);

    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    loupeWidget.setVisible(false);
    changeStyleSheets(QColor(Qt::white));
    setCurrentColor(QColor(Qt::white));
    tempCurrentColor = QColor(Qt::white);
    tempX = 0;
    tempY = 0;
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
        auto cursorPos = QCursor::pos();
        QImage img = QApplication::primaryScreen()->grabWindow(0,cursorPos.x(), cursorPos.y(),1,1).toImage();
        QColor color;
        color.setRgb(img.pixel(0,0));
        setCurrentColor(color);
        tempCurrentColor = color;
        gradationWidget.setVisible(false);
        sliderWidget.setVisible(false);
        loupeWidget.setVisible(false);
        QImage imgPixmap = QApplication::primaryScreen()->grabWindow(0,cursorPos.x()-12, cursorPos.y()-12,24,24).toImage();
        imgPixmap = imgPixmap.scaled(72,72);
        imgPixmap.setPixelColor(36,36,QColor(Qt::red));
        imgPixmap.setPixelColor(36,37,QColor(Qt::red));
        imgPixmap.setPixelColor(37,36,QColor(Qt::red));
        imgPixmap.setPixelColor(37,37,QColor(Qt::red));
        loupeLabel.setPixmap(QPixmap::fromImage(imgPixmap));
        gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), m_currentColor));
        lineEditX.setText(QString::number(cursorPos.x()));
        lineEditY.setText(QString::number(cursorPos.y()));
        adjustSize();
        (followCursor) ? showPos(cursorPos) : showPos(posWin);
        slotCopyBuffer(m_currentColor);
        emit visibleChanged();
    }
}

void PopUpColor::slotShow()
{
    changeLabelText(m_currentColor);
    changeStyleSheets(m_currentColor);
    showPos(posWin);
    emit visibleChanged();
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
    gShortcutShow.setShortcut(QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString()));
    comboBox.setCurrentIndex(typeCopyBuffer);
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

void PopUpColor::changeStyleSheets(const QColor &color)
{
    comboBox.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradationCombobox(color));
    label.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCodeLabel(color));
    labelX.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCodeLabel(color));
    labelY.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCodeLabel(color));
    lineEditX.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfLineEdit(color));
    lineEditY.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfLineEdit(color));
    loupeButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfLoupeButton(loupeWidget.isVisible(),color));
    pickerButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfPicker(dummyTransparentWindow.isVisible(), color));
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), color));
    copyButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCopy(color));
    closeButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfCloseButton(color));
    sliderWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfSliderWidget(color));
    gradationWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradationWidget(color));
    loupeWidget.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfLoupeWidget(color));
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
    if(!sliderHue.isSliderDown() && !sliderSaturation.isSliderDown() && !sliderLightness.isSliderDown()){
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

void PopUpColor::coordinatePressed()
{
    tempX = lineEditX.text().toInt();
    tempY = lineEditY.text().toInt();
    QImage img = QApplication::primaryScreen()->grabWindow(0,tempX,tempY,1,1).toImage();
    QColor color;
    color.setRgb(img.pixel(0,0));
    QImage imgPixmap = QApplication::primaryScreen()->grabWindow(0,tempX-12,tempY-12,24,24).toImage();
    imgPixmap = imgPixmap.scaled(72,72);
    imgPixmap.setPixelColor(36,36,QColor(Qt::red));
    imgPixmap.setPixelColor(36,37,QColor(Qt::red));
    imgPixmap.setPixelColor(37,36,QColor(Qt::red));
    imgPixmap.setPixelColor(37,37,QColor(Qt::red));
    loupeLabel.setPixmap(QPixmap::fromImage(imgPixmap));
    setCurrentColor(color);
    tempCurrentColor = color;
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), m_currentColor));
    slotCopyBuffer(m_currentColor);
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
    if(dummyTransparentWindow.isVisible()) setCurrentColor(tempCurrentColor);;
    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    loupeWidget.setVisible(false);
    gradationButton.setStyleSheet(PopUpColorStyleSheetHelper::getStyleSheetOfGradation(gradationWidget.isVisible(), m_currentColor));
    adjustSize();
    hide();
    emit visibleChanged();
}

void PopUpColor::gradationButtonClicked()
{
    gradationWidget.setVisible(!gradationWidget.isVisible());
    sliderWidget.setVisible(!sliderWidget.isVisible());
    changeStyleSheets(m_currentColor);
    adjustSize();
}
