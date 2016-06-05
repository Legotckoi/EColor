#include "versionchecker.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QDebug>

class VersionCheckerPrivate
{
public:
    QNetworkAccessManager *networkManager;
    VersionData *versionData;
    QUrl url;

    VersionCheckerPrivate(){}
};

VersionChecker::VersionChecker(QObject *parent) :
    QObject(parent),
    checker(new VersionCheckerPrivate)
{
    checker->networkManager = new QNetworkAccessManager();
    checker->versionData = new VersionData();

    connect(checker->networkManager, &QNetworkAccessManager::finished, this, &VersionChecker::onResult);
    connect(&timerCheck, &QTimer::timeout, this, &VersionChecker::slotUpdate);
}

VersionChecker::VersionChecker(QObject *parent, QString &softName, int majVersion, int minVersion, int patVersion) :
    QObject(parent),
    checker(new VersionCheckerPrivate)
{
    checker->networkManager = new QNetworkAccessManager();
    checker->versionData->setSoftName(softName);
    checker->versionData->setMajVersion(majVersion);
    checker->versionData->setMinVersion(minVersion);
    checker->versionData->setPatVersion(patVersion);

    connect(checker->networkManager, &QNetworkAccessManager::finished, this, &VersionChecker::onResult);
    connect(&timerCheck, &QTimer::timeout, this, &VersionChecker::slotUpdate);
}

VersionChecker::VersionChecker(QObject *parent, QString &softName, QString fullVersion) :
    QObject(parent),
    checker(new VersionCheckerPrivate)
{
    checker->networkManager = new QNetworkAccessManager();
    checker->versionData = new VersionData();
    checker->versionData->setSoftName(softName);
    setFullVersion(fullVersion);

    connect(checker->networkManager, &QNetworkAccessManager::finished, this, &VersionChecker::onResult);
    connect(&timerCheck, &QTimer::timeout, this, &VersionChecker::slotUpdate);
}

VersionChecker::~VersionChecker()
{
    delete checker;
}

void VersionChecker::setSoftName(QString softName)
{
    checker->versionData->setSoftName(softName);
}

void VersionChecker::setMajVersion(int majVersion)
{
    checker->versionData->setMajVersion(majVersion);
}

void VersionChecker::setMinVersion(int minVersion)
{
    checker->versionData->setMinVersion(minVersion);
}

void VersionChecker::setPatVersion(int patVersion)
{
    checker->versionData->setPatVersion(patVersion);
}

void VersionChecker::setFullVersion(QString fullVersion)
{
    QStringList list = fullVersion.split(".");
    checker->versionData->setMajVersion(list.at(0).toInt());
    checker->versionData->setMinVersion(list.at(1).toInt());
    checker->versionData->setPatVersion(list.at(2).toInt());
}

void VersionChecker::setUrl(QUrl url)
{
    checker->url = url;
}

void VersionChecker::setStringUrl(QString str)
{
    setUrl(QUrl(str));
}

void VersionChecker::startChecker()
{
    timerCheck.start(50);
}

void VersionChecker::stopChecker()
{
    timerCheck.stop();
}

void VersionChecker::slotUpdate()
{
    getData();
    timerCheck.start(20*60*1000);
}

void VersionChecker::onResult(QNetworkReply *reply)
{
    if(!reply->error()){
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

        QJsonObject jo = document.object();
        if(jo.value("software").toString() == checker->versionData->softName()){
            if(jo.value("ver_maj").toInt() > checker->versionData->majVersion()){
                stopChecker();
                emit signalNewVersion(QString(jo.value("full_version").toString()));
            } else if(jo.value("ver_maj").toInt() == checker->versionData->majVersion()){
                if(jo.value("ver_min").toInt() > checker->versionData->minVersion()){
                    stopChecker();
                    emit signalNewVersion(QString(jo.value("full_version").toString()));
                } else if(jo.value("ver_min").toInt() == checker->versionData->minVersion()){
                    if(jo.value("ver_pat").toInt() > checker->versionData->patVersion()){
                        stopChecker();
                        emit signalNewVersion(QString(jo.value("full_version").toString()));
                    } else {
                        stopChecker();
                    }
                }
            }
        }
    }
    reply->deleteLater();
}

void VersionChecker::getData()
{
    checker->networkManager->get(QNetworkRequest(checker->url));
}


VersionData::VersionData(QObject *parent) :
    QObject(parent)
{

}

VersionData::VersionData(const VersionData &other) :
    QObject(0),
    m_softName(other.m_softName),
    m_majVersion(other.m_majVersion),
    m_minVersion(other.m_minVersion),
    m_patVersion(other.m_patVersion)
{

}

QString VersionData::softName() const
{
    return m_softName;
}

int VersionData::majVersion() const
{
    return m_majVersion;
}

int VersionData::minVersion() const
{
    return m_minVersion;
}

int VersionData::patVersion() const
{
    return m_patVersion;
}

void VersionData::setSoftName(const QString &value)
{
    m_softName = value;
    emit dataChanged();
}

void VersionData::setMajVersion(const int value)
{
    m_majVersion = value;
    emit dataChanged();
}

void VersionData::setMinVersion(const int value)
{
    m_minVersion = value;
    emit dataChanged();
}

void VersionData::setPatVersion(const int value)
{
    m_patVersion = value;
    emit dataChanged();
}
