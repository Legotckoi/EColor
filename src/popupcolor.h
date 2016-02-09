#ifndef POPUPCOLOR_H
#define POPUPCOLOR_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QToolButton>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QComboBox>
#include <QSlider>

#include "transparentwindow.h"
#include "gradationlabel.h"
#include "settings.h"

class PopUpColor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
    Q_PROPERTY(QColor currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

    float getPopupOpacity() const;
    QColor getCurrentColor() const;

signals:
    void currentColorChanged(const QColor &color);
    void previousPositionChanged(const QPoint &previousPosition);
    void hotKeysSettingsReloading(const QKeySequence &keys, const bool settingsAllowScreenShots);

public:
    explicit PopUpColor(QWidget *parent = 0);
    void saveSettings();
    ~PopUpColor();

public slots:
    void onHotKeyShowPressed();
    void onHotKeyPixmapPressed();
    void slotShow();
    void slotHide();
    void showPos(QPoint point);
    void reloadSettings();
    void setPopupOpacity(float opacity);
    void setCurrentColor(QColor color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void pickerButtonClicked();
    void copyButtonClicked();
    void gradationButtonClicked();
    void hideAnimation();
    void changeIndexComboBoxColor(int index);
    void backColor();
    void saveColor();
    void sliderPress();
    void sliderRelease();
    void setHue(int value);
    void setSaturation(int value);
    void setLightness(int value);
    void changeStyleSheets(const QColor &color);
    void changeLabelText(const QColor &color);
    void changeSliders(const QColor &color);
    void slotCopyBuffer(const QColor &color);
    void updateStyleSheets();
    void setPreviousPosition(const QPoint &previousPosition);


private:
    QPoint previousPosition() const;

    // Properties
    float popupOpacity;
    QPropertyAnimation animation;
    QColor currentColor;
    QColor tempCurrentColor;
    // Interface
    QWidget popUpWidget;
    QGridLayout layoutPopUp;
    QLabel label;
    QToolButton closeButton;
    QToolButton pickerButton;
    QToolButton gradationButton;
    QToolButton copyButton;
    QComboBox comboBox;
    QGridLayout layout;
    QPoint posWin;
    // Pipette
    TransparentWindow dummyTransparentWindow;
    // Slider
    QWidget sliderWidget;
    QGridLayout layoutSlider;
    QSlider sliderHue;
    QSlider sliderSaturation;
    QSlider sliderLightness;
    QLabel imgHue;
    QLabel imgSaturation;
    QLabel imgLightness;
    bool sliderPressed;
    // Gradation
    QWidget gradationWidget;
    QGridLayout layoutGradation;
    GradationLabel labelGradation[COUNT_GRADATION];


    // Переменные для работы с горячими клавишами
    QKeySequence    keys;
    bool            copyBuffer;
    int             typeCopyBuffer;
    bool            followCursor;

    QPoint m_previousPosition;
    bool m_leftMouseButtonPressed;
};

#endif // POPUPCOLOR_H
