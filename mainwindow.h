#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QCloseEvent>
#include <QSystemTrayIcon>
#include "windows.h"

#include "settings.h"
#include "versionchecker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setColor(const QColor &color);
    bool checkKeySequence();
    bool checkKeyModifier(QString str);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showDialogUpdate(QString newVersion);

    void on_about_triggered();
    void on_quit_triggered();
    void on_config_triggered();

private:
    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
    QColor          *color;
    QColorDialog    *colorDialog;
    VersionChecker  *versionChecker;

    bool needToClose = false;

};

#endif // MAINWINDOW_H
