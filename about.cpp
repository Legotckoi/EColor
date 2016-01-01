#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>
#include <QUrl>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setModal(true);
    ui->labelVersion->setText("EColor v" + QString(VER));
}

About::~About()
{
    delete ui;
}

void About::on_label_3_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void About::on_label_2_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
