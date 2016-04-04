#include "popupcolorstylesheethelper.h"

#include <QString>
#include <QColor>
#include <QDebug>

QString PopUpColorStyleSheetHelper::getStyleSheetOfPicker(const bool isActive, const QColor &color)
{
    QColor backgroundColor{correctedColor(color)};

    if (isColorLight(color)) {
        if (isActive)
            backgroundColor = backgroundColor.darker();

        return "QToolButton { image: url(:/images/eyedropper-black.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    } else {
        if (isActive)
            backgroundColor = backgroundColor.lighter();

        return "QToolButton { image: url(:/images/eyedropper.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfLoupeButton(const bool isActive, const QColor &color)
{
    QColor backgroundColor{correctedColor(color)};

    if (isColorLight(color)) {
        if (isActive)
            backgroundColor = backgroundColor.darker();

        return "QToolButton { image: url(:/images/loupe-black.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    } else {
        if (isActive)
            backgroundColor = backgroundColor.lighter();

        return "QToolButton { image: url(:/images/loupe.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfGradation(const bool isActive, const QColor &color)
{
    QColor backgroundColor{correctedColor(color)};

    if (isColorLight(color)) {
        if (isActive)
            backgroundColor = backgroundColor.darker();

        return "QToolButton { image: url(:/images/invert-colors-black.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    } else {
        if (isActive)
            backgroundColor = backgroundColor.lighter();

        return "QToolButton { image: url(:/images/invert-colors.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + backgroundColor.name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfCopy(const QColor &color)
{
    if (isColorLight(color)) {
        return "QToolButton { image: url(:/images/content-copy-black.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + correctedColor(color).name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    } else {
        return "QToolButton { image: url(:/images/content-copy.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "padding: 6px;"
               "border: none;"
               "border-radius: 2px;"
               "background-color: " + correctedColor(color).name() + "; }"
               "QToolButton:pressed { background-color: transparent; }";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfGradationCombobox(const QColor &color)
{
    QString fontColor = (isColorLight(color)) ? "black" : "white";
    return "QComboBox { color: " + fontColor + "; background-color: " + correctedColor(color).name() + "; "
           "border: none; border-radius: 2px;"
           "padding: 6px;"
           "font-size: 14px; "
           "alternate-background-color: " + color.name() + ";}"
           "QComboBox::drop-down {border: none;} "
           "QComboBox::down-arrow {image: url(noimg); border: none;}"
           "QComboBox QAbstractItemView { "
           "border: none;"
           "color: " + fontColor + "; "
           "background-color: " + correctedColor(color).name() + "; "
           "padding: 6px;"
           "margin-top: 0px;"
           "margin-left: 0px;"
           "margin-right: 0px;"
           "margin-bottom: -12px;"
           "selection-color: " + color.name() + ";"
           "alternate-background-color: " + color.name() + ";}";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfCodeLabel(const QColor &color)
{
    if (isColorLight(color)) {
        return "QLabel { color: black; border: none; font-size: 16px; }";
    } else {
        return "QLabel { color: white; border: none; font-size: 16px; }";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfCloseButton(const QColor &color)
{
    if (isColorLight(color)) {
        return "QToolButton { image: url(:/images/close-circle-outline-black.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "border: none;}"
               "QToolButton:pressed { image: url(:/images/close-circle-black.png);}";
    } else {
        return "QToolButton { image: url(:/images/close-circle-outline.png);"
               "icon-size: 16px;"
               "height: 16px;"
               "width: 16px;"
               "border: none;}"
               "QToolButton:pressed { image: url(:/images/close-circle.png);}";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfSliderWidget(const QColor &color)
{
    return "QWidget { background-color: " + color.name() + ";"
           "border: 1px solid " + correctedColor(color).name() + ";"
           "margin-left: 6px;"
           "margin-right: 6px;}";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfGradationWidget(const QColor &color)
{
    return "QWidget { background-color: transparent;"
           "border: 1px solid " + correctedColor(color).name() + ";"
           "margin-left: 6px;"
           "margin-right: 6px;}";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfPopUpWidget(const QColor &color)
{
    return "QWidget { background-color: " + color.name() + ";"
           "border: 1px solid " + correctedColor(color).name() + ";"
           "border-radius: 2px;}";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfSlider(const QColor &color)
{
    QColor correctColor = correctedColor(color);
    if(isColorLight(color)){
        return "QSlider {border: none; margin: 0px; padding: 2px;}"
               "QSlider::groove:horizontal { border:none; height: 4px; background-color: " + correctColor.name() + ";}"
               "QSlider::handle:horizontal { border: 2px solid " + correctColor.name() + "; height: 12px; margin: -6px 0; width: 12px; background: " + correctColor.darker().name() + "; border-radius: 8px;}";
    } else {
        return "QSlider {border: none; margin: 0px; padding: 2px;}"
               "QSlider::groove:horizontal { border:none; height: 4px; background-color: " + correctColor.name() + ";}"
               "QSlider::handle:horizontal { border: 2px solid " + correctColor.name() + "; height: 12px; margin: -6px 0; width: 12px; background: " + correctColor.lighter().name() + "; border-radius: 8px;}";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfLoupeWidget(const QColor &color)
{
    return "QWidget { background-color: " + color.name() + ";"
           "border: 1px solid " + correctedColor(color).name() + ";"
           "margin-top: 6px;"
           "margin-bottom: 6px;}";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfImageHue(const QColor &color)
{
    if (isColorLight(color)) {
        return "QLabel {border: none; margin: 0; image: url(:/images/palette-black.png);}";
    } else {
        return "QLabel {border: none; margin: 0; image: url(:/images/palette.png);}";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfImageLightness(const QColor &color)
{
    if (isColorLight(color)) {
        return "QLabel {border: none; margin: 0; image: url(:/images/brightness-black.png);}";
    } else {
        return "QLabel {border: none; margin: 0; image: url(:/images/brightness.png);}";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfLineEdit(const QColor &color)
{
    if (isColorLight(color)) {
        return "QLineEdit { width: 56px;"
               "height: 24px;"
               "background: transparent;"
               "color: #000000;"
               "font-size: 16px;}";
    } else {
        return "QLineEdit { width: 56px;"
               "height: 24px;"
               "background: transparent;"
               "color: #ffffff;"
               "font-size: 16px;}";
    }
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfLoupeLabel()
{
    return "QLabel { width: 96px; height: 96px; }";
}

QString PopUpColorStyleSheetHelper::getStyleSheetOfImageSaturation(const QColor &color)
{
    if (isColorLight(color)) {
        return "QLabel {border: none; margin: 0; image: url(:/images/contrast-circle-black.png);}";
    } else {
        return "QLabel {border: none; margin: 0; image: url(:/images/contrast-circle.png);}";
    }
}

bool PopUpColorStyleSheetHelper::isColorLight(const QColor &color)
{
    return color.lightnessF() > 0.7;
}

QColor PopUpColorStyleSheetHelper::correctedColor(const QColor &color)
{
    QColor c;
    qreal lightness = color.lightnessF();
    if(lightness < 0.15){
        c.setHslF(color.hslHueF(),color.hslSaturationF(),0.25);
    } else if(lightness < 0.5) {
        c.setHslF(color.hslHueF(), color.hslSaturationF(), lightness+0.15);
    } else if(lightness < 0.85){
        c.setHslF(color.hslHueF(), color.hslSaturationF(), lightness-0.15);
    } else {
        c.setHslF(color.hslHueF(), color.hslSaturationF(), 0.75);
    }
    return c;
}
