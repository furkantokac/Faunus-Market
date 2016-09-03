#ifndef QJSONCONFIG_H
#define QJSONCONFIG_H

#include <qjsonhandler.h>

#include <QDir>
#include <QStringList>

class QJsonConfig : public QJsonHandler
{
    Q_OBJECT
public:
    explicit QJsonConfig(QJsonHandler *parent = 0);

    void initConfigFile();
    void setDefault();
    void appInstalled(QString installedAppName);
    void appUninstalled(QString uninstalledAppName);

    QStringList findInstalledApps();
    QStringList findInstalledDeps();

signals:

public slots:
};

#endif // QJSONCONFIG_H
