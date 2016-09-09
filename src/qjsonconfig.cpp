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

    if( os=="windows" )
    {
        config.write(
                    "{"
                    "\"installed-apps\": [\"Faunus Market\"],"
                    "\"installed-deps\": [\"\"],"
                    "\"apps-config-url\": [\"https://raw.github.com/furkantokac/Faunus-Config/master/apps-config.json\"],"
                    "\"install-path\": [\"%ProgramFiles%/Faunus\"]"
                    "}"
                    );
    }
    else
    {
        config.write(
                    "{"
                    "\"installed-apps\": [\"Faunus Market\"],"
                    "\"installed-deps\": [\"\"],"
                    "\"apps-config-url\": [\"https://raw.github.com/furkantokac/Faunus-Config/master/apps-config.json\"],"
                    "\"install-path\": [\"~/Faunus/\"]"
                    "}"
                    );
    }
    config.close();

    // Functions from QJsonHandlera
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

void  QJsonConfig::createDesktopShortcutLinux(QString name, QString comment, QString exec, QString icon)
{
    QFile shortcutFile(name+".desktop");
    if( !shortcutFile.open(QIODevice::WriteOnly) )
    {
        qDebug() << "Unable to create desktop entry file.";
        return;
    }
    shortcutFile.write("shortcut");
    shortcutFile.close();
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

QString QJsonConfig::getDir(QString dirName)
{
    if(dirName=="home")
        return QStandardPaths::writableLocation( QStandardPaths::HomeLocation )+"/";
    else if(dirName=="desktop")
        return QStandardPaths::writableLocation( QStandardPaths::DesktopLocation )+"/";
    else if(dirName=="cache")
        return QStandardPaths::writableLocation( QStandardPaths::CacheLocation )+"/";
    else if(dirName=="config")
        return QStandardPaths::writableLocation( QStandardPaths::ConfigLocation )+"/";
    else if(dirName=="data")
        return QStandardPaths::writableLocation( QStandardPaths::DataLocation )+"/";
    else if(dirName=="faunus")
        return "";
    else
        return "";
}
