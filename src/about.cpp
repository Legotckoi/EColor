#include "about.h"
#include "ui_about.h"
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setModal(true);
    ui->labelVersion->setText("EColor v" + QString(APP_VERSION));
    connect(ui->label_2, &QLabel::linkActivated,[=](const QString &link){QDesktopServices::openUrl(QUrl(link));});
    connect(ui->label_3, &QLabel::linkActivated,[=](const QString &link){QDesktopServices::openUrl(QUrl(link));});
}

About::~About()
{
    delete ui;
}

void About::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
    this->deleteLater();
}
