#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QSettings>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QDebug>
#include "settings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    setModal(true);

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->keySequenceEdit->setKeySequence(QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString()));
    ui->checkFollowCursor->setChecked(settings.value(SETTINGS_FOLLOW_CURSOR, true).toBool());
    ui->checkCopyBuffer->setChecked(settings.value(SETTINGS_COPY_BUFF, true).toBool());
    ui->cBoxBufferType->setCurrentIndex(settings.value(SETTINGS_TYPE_BUFF, 0).toInt());

#ifdef Q_OS_WIN32
    ui->checkAutoRun->setChecked(settings.value(SETTINGS_AUTORUN, false).toBool());
#else
    QString autostartPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + QLatin1String("/autostart");
    QDir autorunDir(autostartPath);
    if(!autorunDir.exists()) autorunDir.mkpath(autostartPath);
    QFile autorun(autostartPath + QLatin1String("/EColor.desktop"));
    ui->checkAutoRun->setChecked(autorun.exists());
    autorun.close();
#endif

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [=](){saveSettings();hide();deleteLater();});
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [=](){hide();deleteLater();});
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::saveSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(KEY_SEQUENCE_PIXEL, ui->keySequenceEdit->keySequence().toString());
    settings.setValue(SETTINGS_FOLLOW_CURSOR, ui->checkFollowCursor->isChecked());
    settings.setValue(SETTINGS_COPY_BUFF, ui->checkCopyBuffer->isChecked());
    settings.setValue(SETTINGS_TYPE_BUFF, ui->cBoxBufferType->currentIndex());
    settings.sync();

#ifdef Q_OS_WIN32
    settings.setValue(SETTINGS_AUTORUN, ui->checkAutoRun->isChecked());
    //Добавляем в автозагрузку пользователя
    QSettings autorun("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(ui->checkAutoRun->isChecked()) {
        autorun.setValue(APPLICATION_NAME, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        autorun.sync();
    } else {
        autorun.remove(APPLICATION_NAME);
    }
#else
    QString autostartPath = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).at(0) + QLatin1String("/autostart");
    QFile autorunFile(autostartPath + QLatin1String("/EColor.desktop"));
    if(ui->checkAutoRun->isChecked()) {
        if(!autorunFile.exists()){
            if(autorunFile.open(QFile::WriteOnly)){
                QString autorunContent("[Desktop Entry]\n"
                                       "Type=Application\n"
                                       "Exec=" + QDir::toNativeSeparators(QCoreApplication::applicationFilePath()) + "\n"
                                       "Hidden=false\n"
                                       "NoDisplay=false\n"
                                       "X-GNOME-Autostart-enabled=true\n"
                                       "Name[en_GB]=EColor\n"
                                       "Name=EColor\n"
                                       "Comment[en_GB]=EColor\n"
                                       "Comment=EColor\n");
                QTextStream outStream(&autorunFile);
                outStream << autorunContent;
                autorunFile.setPermissions(QFileDevice::ExeUser|QFileDevice::ExeOwner|QFileDevice::ExeOther|QFileDevice::ExeGroup|
                                       QFileDevice::WriteUser|QFileDevice::ReadUser);
                autorunFile.close();
            }
        }
    } else {
        if(autorunFile.exists()) autorunFile.remove();
    }
#endif

    emit reloadKeySequence();
}

void DialogSettings::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply) {
        saveSettings();
    }
}

void DialogSettings::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
    this->deleteLater();
}
