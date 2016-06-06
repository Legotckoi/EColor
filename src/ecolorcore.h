#ifndef ECOLOCORE_H
#define ECOLOCORE_H

#include <QObject>
#include <QSystemTrayIcon>
#include "versionchecker.h"

class PopUpColor;
class PopUpMessage;
class NativeEventFilter;
class QAction;

class EColorCore : public QObject
{
    Q_OBJECT
public:
    explicit EColorCore(QObject *parent = 0);
    ~EColorCore();

private slots:
    void showDialogUpdate(QString newVersion);
#ifdef Q_OS_WIN32
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif

private:
    QSystemTrayIcon     trayIcon;
    VersionChecker      versionChecker;
    PopUpColor          *popUpColor;
    QAction             *actionShow ;
};

#endif // ECOLOCORE_H
