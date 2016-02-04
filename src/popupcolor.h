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

public:
    explicit PopUpColor(QWidget *parent = 0);
    void saveSettings();
    ~PopUpColor();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
    void show();
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
    void hideAnimation();
    void changeIndexComboBoxColor(int index);
    void backColor();
    void saveColor();
    void changeStyleSheets(const QColor &color);
    void changeLabelText(const QColor &color);
    void changeSliders();
    void slotCopyBuffer(const QColor &color);
    void slotGradationButtonClicked();
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
    QComboBox comboBox;
    QToolButton pickerButton;
    QToolButton closeButton;
    QToolButton gradationButton;
    QGridLayout layout;
    QPoint posWin;
    // Pipette
    TransparentWindow dummyTransparentWindow;
    // Slider
    QWidget sliderWidget;
    QGridLayout layoutSlider;
    QSlider sliderSaturation;
    QSlider sliderBrightness;
    QLabel labelSaturation;
    QLabel labelBrightness;
    // Gradation
    QWidget gradationWidget;
    QGridLayout layoutGradation;
    GradationLabel label_10;
    GradationLabel label_20;
    GradationLabel label_30;
    GradationLabel label_40;
    GradationLabel label_50;
    GradationLabel label_60;
    GradationLabel label_70;
    GradationLabel label_80;
    GradationLabel label_90;
    GradationLabel label_100;

    // Переменные для работы с горячими клавишами
    QKeySequence    keys;
    bool            copyBuffer;
    int             typeCopyBuffer;
    bool            followCursor;

    unsigned int winKeyModificator(QKeySequence sequence);
    char winHotKey(QKeySequence sequence);
    QPoint m_previousPosition;
    bool m_leftMouseButtonPressed;
};

#endif // POPUPCOLOR_H
