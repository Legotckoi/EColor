#ifndef NATIVEEVENTFILTER_H
#define NATIVEEVENTFILTER_H

#include <QAbstractNativeEventFilter>
#include <QObject>

class QKeySequence;

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit NativeEventFilter(QObject *parent = nullptr);

    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

signals:
    void hotKeyShowPressed();
    void hotKeyPixmapPressed();

public slots:
    void onHotKeysSettingsReloading(const QKeySequence &keys, const bool settingsAllowScreenShots);
};

#endif // NATIVEEVENTFILTER_H
