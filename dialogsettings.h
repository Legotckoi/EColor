#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAbstractButton>
#include <QCloseEvent>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    ~DialogSettings();

signals:
    void reloadKeySequence();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_keySequenceEdit_editingFinished();
    void on_pBScreenPath_clicked();

private:
    Ui::DialogSettings *ui;
    void saveSettings();
};

#endif // DIALOGSETTINGS_H
