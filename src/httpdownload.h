#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QEventLoop>

#include <QUrl>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

class HttpDownload : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownload(QObject *parent = 0);
    ~HttpDownload();

    bool isConnectedToInternet();
    void download(QString requestedUrl, QString fileName, QString dirName);
    void startRequest(QUrl requestedUrl);

signals:
    void downloadFinished(bool error);
    void downloadStarted();
    void dataReceived(qint64 bytesRead, qint64 totalBytes);

    void downloadError(int errorCode);

private slots:
    void httpReadyRead();
    void updateDownloadProgress(qint64,qint64);
    void httpDownloadFinished();

private:
    bool httpRequestAborted;

    int redirectionCounter =0;

    QNetworkAccessManager *manager;
    QUrl url;
    QFile *file;
    QString fullFileName;
    QString downloadDirName;
    QNetworkReply *reply;
};

#endif // HTTPDOWNLOAD_H
