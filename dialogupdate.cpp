#include "dialogupdate.h"
#include "ui_dialogupdate.h"
#include <QDesktopServices>
#include <QUrl>

DialogUpdate::DialogUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpdate)
{
    ui->setupUi(this);
}

DialogUpdate::~DialogUpdate()
{
    delete ui;
}

void DialogUpdate::setNewVersion(QString str)
{
    ui->labelVersion->setText(str);
}

void DialogUpdate::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Yes) {
        QDesktopServices::openUrl(QUrl("http://www.evileg.ru/ecolor"));
        this->hide();
        this->deleteLater();
    } else if(ui->buttonBox->standardButton(button) == QDialogButtonBox::No) {
        this->hide();
        this->deleteLater();
    }
}
