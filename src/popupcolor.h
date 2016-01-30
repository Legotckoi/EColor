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

class PopUpColor : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

    void setPopupOpacity(float opacity);
    float getPopupOpacity() const;

public:
    explicit PopUpColor(QWidget *parent = 0);
    ~PopUpColor();

protected:
    void paintEvent(QPaintEvent *event);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

public slots:
    void setPopupColor(const QColor& color);
    void show();
    void showPos(QPoint point);
    void reloadSettings();

private slots:
    void dropperbuttonClicked();
    void hideAnimation();
    void changeIndexComboBoxColor(int index);

private:
    CodeLabel label;
    QComboBox comboBox;
    QToolButton dropperButton;
    QToolButton closeButton;
    QGridLayout layout;
    QPropertyAnimation animation;
    float popupOpacity;

    QColor currentColor;

    // Переменные для работы с горячими клавишами
    QKeySequence    keys;
    bool            copyBuffer;
    int             typeCopyBuffer;
    bool            followCursor;

    void setColor(const QColor &color);
    void changeStyleSheets();
    void setLabelText();
    void slotCopyBuffer();
    unsigned int winKeyModificator(QKeySequence sequence);
    char winHotKey(QKeySequence sequence);
};

#endif // POPUPCOLOR_H
