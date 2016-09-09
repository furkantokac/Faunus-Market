#ifndef QJSONCONFIG_H
#define QJSONCONFIG_H

#include <qjsonhandler.h>

#include <QDir>
#include <QStringList>
#include <QStandardPaths>

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

    void createDesktopShortcutLinux(QString name, QString comment, QString exec, QString icon);

    QString getDir(QString dirName);

signals:

public slots:

private:
    QString os;
    QString faunusDir ;
};

#endif // QJSONCONFIG_H
