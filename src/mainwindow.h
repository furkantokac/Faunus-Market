#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <httpdownload.h>
#include <qjsonhandler.h>
#include <qjsonconfig.h>

#include <QDebug>
#include <QDir>
#include <QFile>

#include <QTreeWidget>
#include <QByteArray>
#include <QTextEdit>

#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

#include <QProgressDialog>

// Downloaded file types
#define APPLIST 100
#define APP 101


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnInstallApp_clicked();
    void on_btnRefreshApplist_clicked();
    void on_btnDownloadApp_clicked();

    void finishDownload(int error, QString errorStr);
    void startDownload();
    void updateDownloadProgress(qint64 bytesRead, qint64 totalBytes);

private:
    Ui::MainWindow *ui;

    void loadTreeWidget(QTreeWidget *tw);
    void apptop(QTextEdit *txe, QString text);
    void uiSetEnabled(bool state);
    void updateStatus(QString msg);

    QString downloadedAppName;
    int whatIsDownloaded=0; // Will be assigned defined file types

    HttpDownload downloader;
    QJsonConfig marketConfig;
    QJsonHandler appsConfig;

    // File Locations and Names
    const QString appsConfigFile = "./cache/apps-config.json";

    // Faunus Market Constants
    const int marketVersionCode = 1;
    const QString marketVersionNo = "0.0.1";
};

#endif // MAINWINDOW_H
