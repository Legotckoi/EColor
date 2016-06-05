#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
#include <QFile>
#include <QTranslator>
#include <QLocale>

#include "settings.h"
#include "ecolorcore.h"
#include "QStyleFactory"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);
    QApplication a(argc, argv);

    QSystemSemaphore semaphore("EColor Semaphore", 1);
    semaphore.acquire();

#ifndef Q_OS_WIN32
    QSharedMemory nix_fix_shared_memory("EColor Shared Memory");
        if(nix_fix_shared_memory.attach()){
            nix_fix_shared_memory.detach();
        }
#endif

    QSharedMemory sharedMemory("EColor Shared Memory");
    bool is_running;
    if (sharedMemory.attach()){
        is_running = true;
    }else{
        sharedMemory.create(1);
        is_running = false;
    }
    semaphore.release();

    a.setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    QColor textColor(0xf2, 0xf2, 0xf2);
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, textColor);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::Text, textColor);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, textColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, Qt::red);
    darkPalette.setColor(QPalette::LinkVisited, Qt::red);

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    a.setPalette(darkPalette);

    a.setStyleSheet("QToolTip { color: #ffffff;"
                    "background-color: rgba(0,0,0,75%);"
                    "border: none;"
                    "padding: 3px;"
                    "border-radius: 2px; }");

    if(is_running){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(QObject::trUtf8("EColor application already running."
                        "\r\nYou able to run only one instance of application."));
        msgBox.exec();
        return 1;
    }
    QTranslator translator;
    translator.load(":/translations/ecolor_" + QLocale::system().name());
    qApp->installTranslator(&translator);
    EColorCore eColorCore;

    return a.exec();
}
