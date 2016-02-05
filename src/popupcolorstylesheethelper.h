#ifndef POPUPCOLORSTYLESHEETHELPER_H
#define POPUPCOLORSTYLESHEETHELPER_H

#include <QtGlobal>

class QString;
class QColor;

class PopUpColorStyleSheetHelper
{
public:
    static QString getStyleSheetOfPicker(const bool isActive, const QString &strColor, const QColor &color);
    static QString getStyleSheetOfGradation(const bool isActive, const QString &strColor, const QColor &color);
    static QString getStyleSheetOfGradationCombobox(const QString &strColor, const QColor &color, const QString &fontColor);
    static QString getStyleSheetOfCodeLabel(const QColor &color);
    static QString getStyleSheetOfCloseButton(const QColor &color);
    static QString getStyleSheetOfSliderWidget(const QColor &color, const QColor &correctedColor);
    static QString getStyleSheetOfGradationWidget(const QColor &correctedColor);
    static QString getStyleSheetOfPopUpWidget(const QColor &color, const QColor &correctedColor);
    static QString getStyleSheetOfSliderBrightness();
    static QString getStyleSheetOfSliderSaturation();

    static bool isColorLight(const QColor &color);
    static qreal correctedLightness(qreal lightness);
};

#endif // POPUPCOLORSTYLESHEETHELPER_H
