#include "qjsonconfig.h"

QJsonConfig::QJsonConfig(QJsonHandler *parent) : QJsonHandler(parent)
{
    fileName = "config.json";
    if( !QFile(fileName).exists() )
    {
        qDebug() << "There is no config file. Im creating one for you.";
        initConfigFile();
        return;
    }

    if( load(fileName) )
        qDebug() << "Config file successfuly loaded.";
}

void QJsonConfig::initConfigFile()
{
    QString os;

#ifdef Q_OS_WIN32
    os = "windows";
#else
    os = "linux";
#endif
    setDefault();
}

void QJsonConfig::setDefault()
{
    QFile config(fileName);
    if( !config.open(QIODevice::WriteOnly) )
    {
        qDebug() << "Unable to create new config file.";
        return;
    }

    config.write(
                "{"
                "\"installed-apps\": [\"Faunus Market\"],"
                "\"installed-deps\": [\"\"],"
                "\"apps-config-url\": [\"https://raw.github.com/furkantokac/Faunus-Config/master/apps-config.json\"]"
                "}"
                );
    config.close();

    // Functions from QJsonHandler
    if( load(fileName) )
    {
        qDebug() << "Config file successfuly created and loaded.";
        save();
    }
}

QStringList QJsonConfig::findInstalledApps()
{
    QStringList temp;
    temp.append("");
    return temp;
}

QStringList QJsonConfig::findInstalledDeps()
{
    QStringList temp;
    temp.append("");
    return temp;
}

void QJsonConfig::appInstalled(QString installedAppName)
{
    append("installed-apps", installedAppName);
    save();
}

void QJsonConfig::appUninstalled(QString uninstalledAppName)
{
    remove("installed-apps", uninstalledAppName);
    save();
}
