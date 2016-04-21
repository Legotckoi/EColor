#include "gradationlabel.h"
#include <QPainter>
#include <cmath>
#include <QDebug>

GradationLabel::GradationLabel(QWidget *parent) : QWidget(parent)
{
    setLayout(&layout);
    lightnessLabel.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    codeLabel.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    layout.addWidget(&lightnessLabel,0,0);
    layout.addWidget(&codeLabel,0,1);

    connect(this, &GradationLabel::currentColorChanged, this, &GradationLabel::changeStyleSheets);
    connect(this, &GradationLabel::currentColorChanged, this, &GradationLabel::changeLabelText);
    connect(this, &GradationLabel::currentColorChanged, this, static_cast<void (GradationLabel::*)(void)>(&GradationLabel::repaint));
    connect(this, &GradationLabel::typeCodeChanged, [this](){ changeLabelText(getCurrentColor()); });

    setCurrentColor(QColor(Qt::white));
    changeStyleSheets();
    adjustSize();
}

qreal GradationLabel::getCurrentLightness() const
{
    return m_currentLightness;
}

QColor GradationLabel::getCurrentColor() const
{
    return m_currentColor;
}

int GradationLabel::typeCode() const
{
    return m_typeCode;
}

void GradationLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor penColor;
    penColor.setHslF(m_currentColor.hslHueF(),
                     m_currentColor.hslSaturationF(),
                     m_currentLightness);

    painter.setBrush(QBrush(penColor));
    painter.setPen(QPen(penColor));

    QRectF rectBackground;
    rectBackground.setX(rect().x());
    rectBackground.setY(rect().y());
    rectBackground.setWidth(rect().width());
    rectBackground.setHeight(rect().height());

    painter.drawRect(rectBackground);
}

void GradationLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        emit colorForCopy(m_currentColor);
    } else {
        emit colorForSet(m_currentColor);
    }
    event->accept();
}

void GradationLabel::changeStyleSheets()
{
    QString fontColor = (m_currentLightness > 0.7) ? "#000000" : "#ffffff";
    codeLabel.setStyleSheet("QLabel { font-size: 14px;"
                            "border: none;"
                            "color: " + fontColor + ";}");
    lightnessLabel.setStyleSheet("QLabel { font-size: 14px;"
                                 "border: none;"
                                 "color: " + fontColor + ";}");
}

void GradationLabel::changeLabelText(const QColor &color)
{
    lightnessLabel.setText(QString::number(round(color.lightnessF()*100)));
    switch (typeCode()) {
    case 0:
        codeLabel.setText(color.name());
        break;
    case 1:
        codeLabel.setText("RGB:\t" +
                      QString::number(color.red()) + " " +
                      QString::number(color.green()) + " " +
                      QString::number(color.blue()));
        break;
    case 2:
        codeLabel.setText("CMYK:\t" +
                      QString::number(color.cyan()) + " " +
                      QString::number(color.magenta()) + " " +
                      QString::number(color.yellow()) + " " +
                      QString::number(color.black()));
        break;
    case 3:
        codeLabel.setText("HSV:\t" +
                      QString::number(color.hsvHue()) + " " +
                      QString::number(round(color.hsvSaturationF()*100)) + "% " +
                      QString::number(round(color.valueF()*100)) + "%");
        break;
    case 4:
        codeLabel.setText("HSL:\t" +
                      QString::number(color.hslHue()) + " " +
                      QString::number(round(color.hslSaturationF()*100)) + "% " +
                      QString::number(round(color.lightnessF()*100)) + "%");
        break;
    default:
        break;
    }
}

void GradationLabel::setCurrentLightness(qreal currentLightness)
{
    if (m_currentLightness == currentLightness)
        return;

    m_currentLightness = currentLightness;
    emit currentLightnessChanged(currentLightness);
}

void GradationLabel::setCurrentColor(QColor currentColor)
{
    if (m_currentColor == currentColor)
        return;

    currentColor.setHslF(currentColor.hslHueF(), currentColor.hslSaturationF(),m_currentLightness);
    m_currentColor = currentColor;
    emit currentColorChanged(currentColor);
}

void GradationLabel::setTypeCode(int typeCode)
{
    if (m_typeCode == typeCode)
        return;

    m_typeCode = typeCode;
    emit typeCodeChanged(typeCode);
}
