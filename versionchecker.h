#ifndef CHECKVERSION_H
#define CHECKVERSION_H

#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QNetworkReply>

class VersionData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString softName READ softName WRITE setSoftName NOTIFY dataChanged)
    Q_PROPERTY(int majVersion READ majVersion WRITE setMajVersion NOTIFY dataChanged)
    Q_PROPERTY(int minVersion READ minVersion WRITE setMinVersion NOTIFY dataChanged)
    Q_PROPERTY(int patVersion READ patVersion WRITE setPatVersion NOTIFY dataChanged)

public:
    explicit VersionData(QObject *parent = 0);
    VersionData(const VersionData &other);

    QString softName() const;
    int     majVersion() const;
    int     minVersion() const;
    int     patVersion() const;

    void setSoftName(const QString &value);
    void setMajVersion(const int value);
    void setMinVersion(const int value);
    void setPatVersion(const int value);

signals:
    void dataChanged();

private:
    QString m_softName;
    int     m_majVersion;
    int     m_minVersion;
    int     m_patVersion;
};

class VersionCheckerPrivate;

class VersionChecker : public QObject
{
    Q_OBJECT
public:
    explicit VersionChecker(QObject *parent = 0);
    VersionChecker(QObject *parent, QString &softName, int majVersion, int minVersion, int patVersion);
    void setSoftName(QString softName);
    void setMajVersion(int majVersion);
    void setMinVersion(int minVersion);
    void setPatVersion(int patVersion);
    void setUrl(QUrl url);
    void setStringUrl(QString str);

signals:
    void signalNewVersion(QString newVersion);

public slots:
    void onResult(QNetworkReply *reply);
    void getData();
    void startChecker();
    void stopChecker();

private slots:
    void slotUpdate();

private:
    VersionCheckerPrivate *checker;
    QTimer *timerCheck;
};

#endif // CHECKVERSION_H
