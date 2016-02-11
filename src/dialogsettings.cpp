#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include "settings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    setModal(true);

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->checkAutoRun->setChecked(settings.value(SETTINGS_AUTORUN, false).toBool());
    ui->keySequenceEdit->setKeySequence(QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString()));
    ui->checkFollowCursor->setChecked(settings.value(SETTINGS_FOLLOW_CURSOR, true).toBool());
    ui->checkCopyBuffer->setChecked(settings.value(SETTINGS_COPY_BUFF, true).toBool());
    ui->cBoxBufferType->setCurrentIndex(settings.value(SETTINGS_TYPE_BUFF, 0).toInt());
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::saveSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(SETTINGS_AUTORUN, ui->checkAutoRun->isChecked());
    settings.setValue(KEY_SEQUENCE_PIXEL, ui->keySequenceEdit->keySequence().toString());
    settings.setValue(SETTINGS_FOLLOW_CURSOR, ui->checkFollowCursor->isChecked());
    settings.setValue(SETTINGS_COPY_BUFF, ui->checkCopyBuffer->isChecked());
    settings.setValue(SETTINGS_TYPE_BUFF, ui->cBoxBufferType->currentIndex());
    settings.sync();

    #ifdef Q_OS_WIN32
    //Добавляем в автозагрузку пользователя
    QSettings autorun("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(ui->checkAutoRun->isChecked()) {
        autorun.setValue(APPLICATION_NAME, QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        autorun.sync();
    } else {
        autorun.remove(APPLICATION_NAME);
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

void DialogSettings::on_buttonBox_accepted()
{
    saveSettings();
    this->hide();
    this->deleteLater();
}

void DialogSettings::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
    this->deleteLater();
}

void DialogSettings::on_keySequenceEdit_editingFinished()
{
    QKeySequence sequence = ui->keySequenceEdit->keySequence();
    if(sequence.count() > 1){
        QStringList list = sequence.toString().split(", ");
        ui->keySequenceEdit->setKeySequence(QKeySequence(list.last()));
    }
}

void DialogSettings::on_buttonBox_rejected()
{
    this->hide();
    this->deleteLater();
}
