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

    setCurrentColor(QColor(Qt::white));
    changeStyleSheets();
    adjustSize();
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

QColor GradationLabel::getCurrentColor() const
{
    return m_currentColor;
}

void GradationLabel::setCurrentColor(QColor color)
{
    color.setHslF(color.hslHueF(), color.hslSaturationF(),m_currentLightness);
    m_currentColor = color;
    emit currentColorChanged(m_currentColor);
}

qreal GradationLabel::getCurrentLightness() const
{
    return m_currentLightness;
}

void GradationLabel::setCurrentLightness(qreal lightness)
{
    m_currentLightness = lightness;
    emit currentLightnessChanged(m_currentLightness);
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
    codeLabel.setText(color.name());
}
