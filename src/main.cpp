#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include <QFile>

#include "settings.h"
#include "ecolorcore.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QApplication a(argc, argv);

    QSystemSemaphore semaphore("EColor Semaphore", 1);
    semaphore.acquire();

    QSharedMemory sharedMemory("EColor Shared Memory");
    bool is_running;
    if (sharedMemory.attach()){
        is_running = true;
    }else{
        sharedMemory.create(1);
        is_running = false;
    }
    semaphore.release();

    QFile styleFile(":/styles/dark.qss");
    styleFile.open(QIODevice::ReadOnly);
    QString qssStr = styleFile.readAll();

    a.setStyleSheet(qssStr);

    if(is_running){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::trUtf8("Приложение EColor уже запущено."
                        "\r\nВы можете запустить только один экземпляр приложения."));
        msgBox.exec();
        return 1;
    }
    EColorCore eColorCore;

    return a.exec();
}
