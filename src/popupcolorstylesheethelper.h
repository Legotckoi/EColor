#ifndef POPUPCOLORSTYLESHEETHELPER_H
#define POPUPCOLORSTYLESHEETHELPER_H

#include <QtGlobal>

class QString;
class QColor;

class PopUpColorStyleSheetHelper
{
public:
    static QString getStyleSheetOfPicker(const bool isActive, const QColor &color);
    static QString getStyleSheetOfGradation(const bool isActive, const QColor &color);
    static QString getStyleSheetOfCopy(const QColor &color);
    static QString getStyleSheetOfGradationCombobox(const QColor &color);
    static QString getStyleSheetOfCodeLabel(const QColor &color);
    static QString getStyleSheetOfCloseButton(const QColor &color);
    static QString getStyleSheetOfSliderWidget(const QColor &color);
    static QString getStyleSheetOfGradationWidget(const QColor &color);
    static QString getStyleSheetOfPopUpWidget(const QColor &color);
    static QString getStyleSheetOfSlider(const QColor &color);
    static QString getStyleSheetOfImageHue(const QColor &color);
    static QString getStyleSheetOfImageSaturation(const QColor &color);
    static QString getStyleSheetOfImageLightness(const QColor &color);

    static bool isColorLight(const QColor &color);
    static QColor correctedColor(const QColor &color);
};

#endif // POPUPCOLORSTYLESHEETHELPER_H
