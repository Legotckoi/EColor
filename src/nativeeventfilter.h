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

public slots:
    void onHotKeysSettingsReloading(const QKeySequence &keys);
};

#endif // NATIVEEVENTFILTER_H
