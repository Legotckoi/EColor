#ifndef ECOLOCORE_H
#define ECOLOCORE_H

#include <QObject>
#include <QSystemTrayIcon>

#include "versionchecker.h"
#include "popupcolor.h"
#include "popupmessage.h"

class EColorCore : public QObject
{
    Q_OBJECT
public:
    explicit EColorCore(QObject *parent = 0);
    ~EColorCore();

signals:

public slots:

private slots:
    void showTriggered();
    void configTriggered();
    void aboutTriggered();
    void quitTriggered();
    void showDialogUpdate(QString newVersion);
#ifdef Q_OS_WIN32
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif

private:
    QSystemTrayIcon *trayIcon;
    VersionChecker *versionChecker;
    PopUpColor *popUpColor;
    PopUpMessage *popUpMessage;
};

#endif // ECOLOCORE_H
