#include "popupcolorstylesheethelper.h"

#include <QString>
#include <QColor>

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

QString PopUpColorStyleSheetHelper::getStyleSheetOfGradationCombobox(const QColor &color)
{
    QString fontColor = (isColorLight(color)) ? "black" : "white";
    return "QComboBox { color: " + fontColor + "; background-color: " + correctedColor(color).name() + "; "
           "border: none; border-radius: 2px;"
           "padding: 6px;"
           "font-size: 14px; }"
           "QComboBox::drop-down {border: none;} "
           "QComboBox::down-arrow {image: url(noimg); border: none;}"
           "QComboBox QAbstractItemView { "
           "border: none;"
           "color: " + fontColor + "; "
           "background-color: " + correctedColor(color).name() + "; "
           "border-radius: 2px;"
           "padding: 6px;"
           "selection-color: " + fontColor + ";"
           "selection-background-color: " + color.name() + ";}";
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
        return "QSlider {border: none; margin: 0px; padding: 0px;}"
               "QSlider::groove:horizontal { border:none; height: 4px; background-color: " + correctColor.name() + ";}"
               "QSlider::handle:horizontal { border: 1px solid " + correctColor.darker().name() + "; height: 14px; margin: -6px 0; width: 14px; background: " + correctColor.darker().name() + "; border-radius: 8px;}";
    } else {
        return "QSlider {border: none; margin: 0px; padding: 0px;}"
               "QSlider::groove:horizontal { border:none; height: 4px; background-color: " + correctColor.name() + ";}"
               "QSlider::handle:horizontal { border: 1px solid " + correctColor.lighter().name() + "; height: 14px; margin: -6px 0; width: 14px; background: " + correctColor.lighter().name() + "; border-radius: 8px;}";
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
    if(color.lightnessF() < 0.15){
        return color.lighter(400);
    } else if (color.lightnessF() < 0.5){
        return color.lighter(130);
    } else if (color.lightnessF() < 0.85){
        return color.darker(130);
    } else {
        return color.darker(150);
    }
}
