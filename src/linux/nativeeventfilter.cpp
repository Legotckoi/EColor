#include "nativeeventfilter.h"
#include <QKeySequence>
#include <QStringList>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xcb/xcb.h>

namespace
{
    QVector<quint32> maskModifiers(){
        return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
    }

    unsigned int X11KeyModificator(const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        unsigned int keyModificator = 0;

        foreach (QString str, list) {
            if(str == "Ctrl"){
                keyModificator += XCB_MOD_MASK_CONTROL;
                continue;
            } else if(str == "Alt"){
                keyModificator += XCB_MOD_MASK_1;
                continue;
            } else if(str == "Shift"){
                keyModificator += XCB_MOD_MASK_SHIFT;
                continue;
            }
        }
        return keyModificator;
    }

    int X11HotKey(Display *display, const QKeySequence &sequence)
    {
        QStringList list = sequence.toString().split("+");
        int hotKey = 0;
        foreach (QString str, list) {
            if(str != "Ctrl" && str != "Alt" && str != "Shift"){
                hotKey = XKeysymToKeycode(display, str.at(0).unicode());
            }
        }
        return hotKey;
    }
    QKeySequence keySequense;
}

NativeEventFilter::NativeEventFilter(QObject *parent) :
    QObject(parent)
{

}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)

    Display * m_display = QX11Info::display();
    xcb_key_press_event_t *keyEvent = 0;
    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t *>(message);
        if ((event->response_type & 127) == XCB_KEY_PRESS){
            keyEvent = static_cast<xcb_key_press_event_t *>(message);
            foreach (quint32 maskMods, maskModifiers()) {
                if((keyEvent->state == X11KeyModificator(keySequense) | maskMods )
                        &&  keyEvent->detail == X11HotKey(m_display,keySequense)){
                    emit hotKeyShowPressed();
                }
            }
        }
    }
    return false;
}

void NativeEventFilter::onHotKeysSettingsReloading(const QKeySequence &keys, const bool settingsAllowScreenShots)
{
    keySequense = keys;
    Q_UNUSED(settingsAllowScreenShots)

    Display * m_display = QX11Info::display();
    Window win = DefaultRootWindow(m_display);
    foreach (quint32 maskMods, maskModifiers()) {
        XGrabKey(m_display, X11HotKey(m_display, keySequense) , X11KeyModificator(keySequense) | maskMods, win,True, GrabModeAsync, GrabModeAsync);
    }
}
