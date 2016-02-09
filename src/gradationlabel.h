#ifndef GRADATIONLABEL_H
#define GRADATIONLABEL_H

#include <QWidget>
#include <QMouseEvent>
#include <QGridLayout>
#include <QLabel>

class GradationLabel : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor currentColor READ getCurrentColor WRITE setCurrentColor NOTIFY currentColorChanged)
    Q_PROPERTY(qreal currentLightness READ getCurrentLightness WRITE setCurrentLightness NOTIFY currentLightnessChanged)

    QColor getCurrentColor() const;
    qreal getCurrentLightness() const;

public:
    explicit GradationLabel(QWidget *parent = 0);


protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void currentColorChanged(const QColor &color);
    void currentLightnessChanged(const qreal lightness);
    void colorForCopy(const QColor  &color);
    void colorForSet(const QColor &color);

public slots:
    void setCurrentColor(QColor color);
    void setCurrentLightness(qreal lightness);

private slots:
    void changeStyleSheets();
    void changeLabelText(const QColor &color);

private:
    // Properties
    QColor  m_currentColor;
    qreal   m_currentLightness;
    // Interface
    QGridLayout layout;
    QLabel lightnessLabel;
    QLabel codeLabel;
};

#endif // GRADATIONLABEL_H
