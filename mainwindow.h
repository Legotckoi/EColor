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
    void initKeySequence();
    void timerTrayTimeout();

    void on_about_triggered();
    void on_quit_triggered();
    void on_config_triggered();

private:
    Ui::MainWindow  *ui;
    QSystemTrayIcon *trayIcon;
    QColorDialog    *colorDialog;
    VersionChecker  *versionChecker;
    QTimer          *timer;

    // Переменные для работы с Хуками
    QStringList     keySequence;
    int             keySequenceLength;
    bool            copyBuffer;
    int             typeCopyBuffer;

    // Закрытие приложения
    bool needToClose = false;

};

#endif // MAINWINDOW_H
