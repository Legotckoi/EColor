#include "nativeeventfilter.h"
#include <windows.h>
#include <QKeySequence>
#include <QStringList>
#include <QString>

namespace
{
    static HWND firstWindowCreatedId = NULL;

    unsigned int winKeyModificator(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        unsigned int keyModificator = 0;

        foreach (QString str, list) {
            if(str == "Ctrl"){
                keyModificator += MOD_CONTROL;
                continue;
            } else if(str == "Alt"){
                keyModificator += MOD_ALT;
                continue;
            } else if(str == "Shift"){
                keyModificator += MOD_SHIFT;
                continue;
            }
        }
        return keyModificator;
    }

    char winHotKey(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        char hotKey = 'E';

        foreach (QString str, list) {
            if(str != "Ctrl" && str != "Alt" && str != "Shift"){
                hotKey = str.at(0).unicode();
            }
        }
        return hotKey;
    }
}

NativeEventFilter::NativeEventFilter(QObject *parent) :
    QObject(parent)
{

}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    MSG* msg = reinterpret_cast<MSG*>(message);
    if (firstWindowCreatedId == NULL)
        firstWindowCreatedId = msg->hwnd;
    if(msg->message == WM_HOTKEY){

        switch (msg->wParam) {
        case 100: {
            emit hotKeyShowPressed();
            return true;
        }
        case 101: {
            emit hotKeyShowPressed();
            return true;
        }
        }
    }
    return false;
}

void NativeEventFilter::onHotKeysSettingsReloading(const QKeySequence &keys, const bool settingsAllowScreenShots)
{
    UnregisterHotKey(firstWindowCreatedId, 101);
    if(settingsAllowScreenShots) {
        RegisterHotKey(firstWindowCreatedId, 101, 0, VK_SNAPSHOT);
    }
    UnregisterHotKey(firstWindowCreatedId, 100);
    RegisterHotKey(firstWindowCreatedId, 100, winKeyModificator(keys), winHotKey(keys));
}
