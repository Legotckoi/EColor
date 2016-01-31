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

#include "codelabel.h"
#include "transparentwindow.h"

class PopUpColor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)
    Q_PROPERTY(QColor currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)

    float getPopupOpacity() const;
    QColor getCurrentColor() const;

signals:
    void currentColorChanged();

public:
    explicit PopUpColor(QWidget *parent = 0);
    void saveSettings();
    ~PopUpColor();

protected:
    void paintEvent(QPaintEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
    void show();
    void showPos(QPoint point);
    void reloadSettings();
    void setPopupOpacity(float opacity);
    void setCurrentColor(QColor color);

private slots:
    void pickerButtonClicked();
    void hideAnimation();
    void changeIndexComboBoxColor(int index);
    void backColor();
    void changeStyleSheets();
    void changeLabelText();
    void slotCopyBuffer();
    void slotHide();

private:
    // Properties
    float popupOpacity;
    QColor currentColor;
    QColor tempCurrentColor;
    // Interface
    CodeLabel label;
    QComboBox comboBox;
    QToolButton pickerButton;
    QToolButton closeButton;
    QGridLayout layout;
    QPropertyAnimation animation;
    QPoint posWin;

    TransparentWindow dummyTransparentWindow;

    // Переменные для работы с горячими клавишами
    QKeySequence    keys;
    bool            copyBuffer;
    int             typeCopyBuffer;
    bool            followCursor;

    void setColor(const QColor &color);
    unsigned int winKeyModificator(QKeySequence sequence);
    char winHotKey(QKeySequence sequence);
};

#endif // POPUPCOLOR_H
