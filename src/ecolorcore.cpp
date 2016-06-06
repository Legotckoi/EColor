#include "ecolorcore.h"
#include "about.h"
#include "settings.h"
#include "dialogsettings.h"
#include "dialogupdate.h"
#include "popupcolor.h"
#include "popupmessage.h"
#include <QMenu>
#include <QAction>
#include <QFont>
#include <QSettings>
#include <QDir>
#include <QApplication>

EColorCore::EColorCore(QObject *parent) : QObject(parent)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if(!settings.contains(KEY_SEQUENCE_PIXEL)){
        settings.setValue(KEY_SEQUENCE_PIXEL, QKeySequence("Ctrl+E").toString());
        settings.sync();
    }

    trayIcon.setToolTip(trUtf8("EColor \n"
                                "Application for grab the pixel color \n"
                                "on the screen"));
    trayIcon.setIcon(QIcon(":/images/ecolor.png"));

    QMenu * menu = new QMenu();
    actionShow = menu->addAction(trUtf8("Show"), [this](){
        (popUpColor->isVisible()) ? popUpColor->slotHide() : popUpColor->slotShow();
    });
    menu->addAction(QIcon(":/images/settings.png"), trUtf8("Settings"), [this](){
        DialogSettings dialogSettings;
        connect(&dialogSettings, &DialogSettings::reloadKeySequence, popUpColor, &PopUpColor::reloadSettings);
        dialogSettings.exec();
    });
    menu->addAction(QIcon(":/images/help.png"), trUtf8("About program"), [this](){ About about; about.exec(); });
    menu->addSeparator();
    menu->addAction(QIcon(":/images/logout.png"), trUtf8("Exit"), [this](){ popUpColor->saveSettings(); trayIcon.hide(); exit(0); });

    trayIcon.setContextMenu(menu);
    trayIcon.show();

#ifdef Q_OS_WIN32
    connect(&trayIcon, &QSystemTrayIcon::activated, this, &EColorCore::iconActivated);
#endif
    connect(&versionChecker, &VersionChecker::signalNewVersion, this, &EColorCore::showDialogUpdate);
    versionChecker.setSoftName(APPLICATION_NAME);
    versionChecker.setFullVersion(APP_VERSION);
    versionChecker.setUrl(QUrl("http://www.evileg.ru/software/ecolor/ecolor.json"));
    versionChecker.startChecker();

    popUpColor = new PopUpColor();
    connect(popUpColor, &PopUpColor::visibleChanged, [this](){(!popUpColor->isVisible())?
                    actionShow->setText(trUtf8("Show")):
                    actionShow->setText(trUtf8("Hide"));});
    popUpColor->reloadSettings();

    if (qApp->arguments().contains("-popup-show", Qt::CaseInsensitive)){
        popUpColor->slotShow();
    }

    PopUpMessage::information(qobject_cast<QWidget *>(this), trUtf8("Application EColor is running"));
}

EColorCore::~EColorCore()
{
    delete popUpColor;
    delete actionShow;
}

#ifdef Q_OS_WIN32
void EColorCore::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        (popUpColor->isVisible())?popUpColor->slotHide():popUpColor->slotShow();
        break;
    default:
        break;
    }
}
#endif

void EColorCore::showDialogUpdate(QString newVersion)
{
    DialogUpdate *dialogUpdate = new DialogUpdate();
    dialogUpdate->setNewVersion(APPLICATION_NAME " v" + newVersion);
    dialogUpdate->show();
}
