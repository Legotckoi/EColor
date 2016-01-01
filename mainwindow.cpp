#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QDebug>
#include <QSettings>

#include "about.h"
#include "dialogsettings.h"
#include "dialogupdate.h"

HHOOK hMouseHook;

LRESULT CALLBACK grabberMouseProc(int Code, WPARAM wParam, LPARAM lParam)
{
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
        if (pMouseStruct != NULL){
            if(wParam == WM_LBUTTONDOWN)
            {
                QWidgetList wl = QApplication::topLevelWidgets ();
                foreach (QWidget *widget, wl)
                {
                    if (MainWindow *mw = qobject_cast<MainWindow *>(widget)) {
                        if(mw->checkKeySequence()){
                            QScreen *screen = QApplication::primaryScreen();
                            QPixmap pixmap = screen->grabWindow( 0 );
                            QImage *img = new QImage;
                            *img = pixmap.toImage();
                            QRgb b = img->pixel(QCursor::pos());
                            QColor c;

                            c.setRgb(b);


                            mw->setColor(c);
                        }
                        break;
                    }
                }
            }
        }
    return CallNextHookEx(hMouseHook, Code, wParam, lParam);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowStaysOnTopHint);
    setFixedWidth(540);
    setFixedHeight(440);

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if(!settings.contains(KEY_SEQUENCE_PIXEL)){
        settings.setValue(KEY_SEQUENCE_PIXEL, QKeySequence("Ctrl+E").toString());
        settings.sync();
    }

    colorDialog = new QColorDialog(this);
    colorDialog->setOptions(QColorDialog::NoButtons);

    ui->gridLayout->addWidget(colorDialog, 0,0,1,1);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("EColor \n"
                         "Приложение для захвата цвета пикселя \n"
                         "на экране монитора");
    trayIcon->setIcon(QIcon(":/images/ecolor.png"));

    QMenu * menu = new QMenu(this);
    QAction * actionShow = new QAction(trUtf8("Показать окно"), this);
    QFont font;
    font.setBold(true);
    actionShow->setFont(font);
    QAction * actionQuit = new QAction(trUtf8("Выход"), this);

    connect(actionShow, SIGNAL(triggered()), this, SLOT(show()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(on_quit_triggered()));

    menu->addAction(actionShow);
    menu->addSeparator();
    menu->addAction(actionQuit);

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    versionChecker = new VersionChecker();
    connect(versionChecker, SIGNAL(signalNewVersion(QString)), this, SLOT(showDialogUpdate(QString)));
    versionChecker->setSoftName(APPLICATION_NAME);
    versionChecker->setMajVersion(VER_MAJOR);
    versionChecker->setMinVersion(VER_MINOR);
    versionChecker->setPatVersion(VER_PATHES);
    versionChecker->setUrl(QUrl("http://www.evileg.ru/software/ecolor/ecolor.json"));
    versionChecker->startChecker();

    HINSTANCE hInstance = GetModuleHandle(NULL);

    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, grabberMouseProc, hInstance, 0);

    if (hMouseHook == NULL)
    {
        qDebug() << "Hook failed";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason){
    case QSystemTrayIcon::Trigger:
        !isVisible() ? show() : hide();
        break;
    default:
        break;
    }
}

void MainWindow::showDialogUpdate(QString newVersion)
{
    DialogUpdate *dialogUpdate = new DialogUpdate();
    dialogUpdate->setNewVersion(APPLICATION_NAME " v" + newVersion);
    dialogUpdate->show();
}

void MainWindow::setColor(const QColor &color)
{
    trayIcon->showMessage("EColor",
                          trUtf8("Цвет пикселя\n") +
                          "HEX: \t" + color.name() + "\n"
                          "RGB: \t" +
                          QString::number(color.red()) + " " +
                          QString::number(color.green()) + " " +
                          QString::number(color.blue()) + "\n"
                          "CMYK: \t" +
                          QString::number(color.cyan()) + " " +
                          QString::number(color.magenta()) + " " +
                          QString::number(color.yellow()) + " " +
                          QString::number(color.black()),
                          QSystemTrayIcon::Information,
                          2000);
    colorDialog->setCurrentColor(color);
}

bool MainWindow::checkKeySequence()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    QStringList list = settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString().split("+");

    switch (list.length()) {
    case 1: {
        return (GetAsyncKeyState(list.at(0).at(0).unicode())) ? true : false;
        break;
    }
    case 2: {
        return (checkKeyModifier(list.at(0)) &&
                GetAsyncKeyState(list.at(1).at(0).unicode())) ? true : false;
        break;
    }
    case 3: {
        return (checkKeyModifier(list.at(0)) &&
                checkKeyModifier(list.at(1)) &&
                GetAsyncKeyState(list.at(2).at(0).unicode())) ? true : false;
        break;
    }
    case 4: {
        return (checkKeyModifier(list.at(0)) &&
                checkKeyModifier(list.at(1)) &&
                checkKeyModifier(list.at(2)) &&
                GetAsyncKeyState(list.at(3).at(0).unicode())) ? true : false;
        break;
    }
    default:
        break;
    }
    return false;
}

bool MainWindow::checkKeyModifier(QString str)
{
    if((str == "Ctrl")){
        return GetAsyncKeyState(VK_CONTROL);
    } else if(str == "Shift"){
        return GetAsyncKeyState(VK_SHIFT);
    } else if(str == "Alt"){
        return GetAsyncKeyState(VK_MENU);
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    if(!needToClose
            && settings.value(SETTINGS_TRAY, false).toBool()
            && this->isVisible()){
        this->hide();
        event->ignore();

        trayIcon->showMessage(APPLICATION_NAME,
                              trUtf8("Приложение свернуто в трей, для того чтобы, "
                                     "развернуть окно приложения, щелкните по иконке приложения в трее"),
                              QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information),
                              2000);
    } else {
        trayIcon->hide();
    }
}

void MainWindow::on_about_triggered()
{
    About *about = new About();
    about->show();
}

void MainWindow::on_quit_triggered()
{
    needToClose = true;
    delete trayIcon;
    this->close();
}

void MainWindow::on_config_triggered()
{
    DialogSettings *dialogSettings = new DialogSettings();
    dialogSettings->show();
}
