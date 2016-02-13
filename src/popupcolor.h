#ifndef POPUPCOLOR_H
#define POPUPCOLOR_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QComboBox>
#include <QSlider>

#include "qglobalshortcut.h"
#include "transparentwindow.h"
#include "gradationlabel.h"
#include "settings.h"

class QGlobalShortcut;

class PopUpColor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ popupOpacity WRITE setPopupOpacity)
    Q_PROPERTY(QColor currentColor READ currentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(QPoint previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

    float popupOpacity() const;
    QColor currentColor() const;

signals:
    void visibleChanged();
    void currentColorChanged(const QColor &color);
    void previousPositionChanged(const QPoint &previousPosition);

public:
    explicit PopUpColor(QWidget *parent = 0);
    void saveSettings();
    ~PopUpColor();

public slots:
    void onHotKeyShowPressed();
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
    void gradationButtonClicked();
    void hideAnimation();
    void changeIndexComboBoxColor(int index);
    void saveColor();
    void setHue(int value);
    void setSaturation(int value);
    void setLightness(int value);
    void changeStyleSheets(const QColor &color);
    void changeLabelText(const QColor &color);
    void changeSliders(const QColor &color);
    void slotCopyBuffer(const QColor &color);
    void setPreviousPosition(const QPoint &previousPosition);


private:
    QPoint previousPosition() const;

    // Properties
    float m_popupOpacity;
    QPropertyAnimation animation;
    QColor m_currentColor;
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
    // Gradation
    QWidget gradationWidget;
    QGridLayout layoutGradation;
    GradationLabel labelGradation[COUNT_GRADATION];

    // Global Hot Key
    QGlobalShortcut gShortcutShow;

    // Переменные для работы с горячими клавишами
    bool            copyBuffer;
    int             typeCopyBuffer;
    bool            followCursor;

    QPoint m_previousPosition;
    bool m_leftMouseButtonPressed;
};

#endif // POPUPCOLOR_H
