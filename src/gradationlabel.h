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
    Q_PROPERTY(int typeCode READ typeCode WRITE setTypeCode NOTIFY typeCodeChanged)

public:
    explicit GradationLabel(QWidget *parent = 0);
    qreal getCurrentLightness() const;
    QColor getCurrentColor() const;
    int typeCode() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void currentColorChanged(const QColor &color);
    void currentLightnessChanged(const qreal lightness);
    void colorForCopy(const QColor  &color);
    void colorForSet(const QColor &color);
    void typeCodeChanged(int typeCode);

public slots:
    void setCurrentLightness(const qreal currentLightness);
    void setCurrentColor(const QColor currentColor);
    void setTypeCode(int typeCode);

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
    int    m_typeCode;
};

#endif // GRADATIONLABEL_H
