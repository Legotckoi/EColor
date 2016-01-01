#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QSettings>
#include <QDir>
#include <QDebug>
#include "settings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);
    setModal(true);

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->checkTray->setChecked(settings.value(SETTINGS_TRAY, false).toBool());
    ui->checkAutoRun->setChecked(settings.value(SETTINGS_AUTORUN, false).toBool());
    ui->keySequenceEdit->setKeySequence(QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString()));
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply) {
        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
        settings.setValue(SETTINGS_TRAY, ui->checkTray->isChecked());
        settings.setValue(SETTINGS_AUTORUN, ui->checkAutoRun->isChecked());
        settings.setValue(KEY_SEQUENCE_PIXEL, ui->keySequenceEdit->keySequence().toString());
        settings.sync();

        #ifdef Q_OS_WIN32
        //Добавляем в автозагрузку пользователя
        QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        if(ui->checkAutoRun->isChecked()) {
            autorun->setValue(APPLICATION_NAME, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
            autorun->sync();
        } else {
            autorun->remove(APPLICATION_NAME);
        }
        delete autorun;
        #endif
    }
}
