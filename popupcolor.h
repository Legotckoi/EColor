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

class PopUpColor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopUpColor(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

public slots:
    void setPopupColor(const QColor& color);
    void show();
    void showPos(QPoint point);
    void reloadSettings();

private slots:
    void dropperbuttonClicked();
    void hideAnimation();
    void hide();

private:
    QLabel label;
    QLabel colorLabel;
    QGroupBox groupBox;
    QVBoxLayout vBox;
    QRadioButton rHex;
    QRadioButton rRGB;
    QRadioButton rCMYK;
    QRadioButton rHSV;
    QToolButton dropperButton;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;
    QTimer *timer;

    QColor currentColor;

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