#include "ecolorcore.h"
#include "about.h"
#include "settings.h"
#include "dialogsettings.h"
#include "dialogupdate.h"
#include "versionchecker.h"
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

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("EColor \n"
                         "Приложение для захвата цвета пикселя \n"
                         "на экране монитора");
    trayIcon->setIcon(QIcon(":/images/ecolor.png"));

    QMenu * menu = new QMenu();
    actionShow = new QAction(trUtf8("Показать"), this);
    QAction *actionConfig = new QAction(trUtf8("Настройки"), this);
    QAction *actionAbout = new QAction(trUtf8("О Приложении"), this);
    QAction *actionQuit = new QAction(trUtf8("Выход"), this);

    connect(actionShow, &QAction::triggered, [=](){(popUpColor->isVisible())?
                    popUpColor->slotHide():popUpColor->slotShow();});
    connect(actionConfig, &QAction::triggered, this, &EColorCore::configTriggered);
    connect(actionAbout, &QAction::triggered, [=](){About about; about.exec();});
    connect(actionQuit, &QAction::triggered, [=](){popUpColor->saveSettings();trayIcon->hide();exit(0);});

    menu->addAction(actionShow);
    menu->addAction(actionConfig);
    menu->addAction(actionAbout);
    menu->addSeparator();
    menu->addAction(actionQuit);

    trayIcon->setContextMenu(menu);
    trayIcon->show();

#ifdef Q_OS_WIN32
    connect(trayIcon, &QSystemTrayIcon::activated, this, &EColorCore::iconActivated);
#endif
    versionChecker = new VersionChecker();
    connect(versionChecker, &VersionChecker::signalNewVersion, this, &EColorCore::showDialogUpdate);
    versionChecker->setSoftName(APPLICATION_NAME);
    versionChecker->setMajVersion(VER_MAJOR);
    versionChecker->setMinVersion(VER_MINOR);
    versionChecker->setPatVersion(VER_PATHES);
    versionChecker->setUrl(QUrl("http://www.evileg.ru/software/ecolor/ecolor.json"));
    versionChecker->startChecker();

    popUpColor = new PopUpColor();
    connect(popUpColor, &PopUpColor::visibleChanged, [=](){(!this->popUpColor->isVisible())?
                    this->actionShow->setText(trUtf8("Показать")):
                    this->actionShow->setText(trUtf8("Скрыть"));});
    popUpColor->reloadSettings();
    PopUpMessage::information(qobject_cast<QWidget *>(this), trUtf8("Приложение EColor запущено"));
}

EColorCore::~EColorCore()
{
    delete popUpColor;
    delete versionChecker;
    delete actionShow;
    delete trayIcon;
}

void EColorCore::configTriggered()
{
    DialogSettings dialogSettings;
    connect(&dialogSettings, &DialogSettings::reloadKeySequence, popUpColor, &PopUpColor::reloadSettings);
    dialogSettings.exec();
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
