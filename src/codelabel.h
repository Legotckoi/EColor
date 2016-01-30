#ifndef CODELABEL_H
#define CODELABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class CodeLabel : public QLabel
{
    Q_OBJECT
public:
    explicit CodeLabel(QWidget *parent = 0);

signals:
    void setPos(QPoint pos);

public slots:

private:
    QPoint posOfWidget;
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};

#endif // CODELABEL_H
