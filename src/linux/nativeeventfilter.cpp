#include "nativeeventfilter.h"
#include <QKeySequence>
#include <QStringList>
#include <QString>

NativeEventFilter::NativeEventFilter(QObject *parent) :
    QObject(parent)
{

}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    return false;
}

void NativeEventFilter::onHotKeysSettingsReloading(const QKeySequence &keys, const bool settingsAllowScreenShots)
{
    Q_UNUSED(keys)
    Q_UNUSED(settingsAllowScreenShots)
}
