#ifndef DIALOGUPDATE_H
#define DIALOGUPDATE_H

#include <QDialog>
#include <QAbstractButton>
#include <QCloseEvent>

namespace Ui {
class DialogUpdate;
}

class DialogUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpdate(QWidget *parent = 0);
    ~DialogUpdate();
    void setNewVersion(QString str);

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogUpdate *ui;
};

#endif // DIALOGUPDATE_H
