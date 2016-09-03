#include "httpdownload.h"

HttpDownload::HttpDownload(QObject *parent) : QObject(parent)
{

}

HttpDownload::~HttpDownload()
{

}

void HttpDownload::httpReadyRead()
{
    // Each time data arrived, write the data to file
    // so decrease ram usage
    if( file )
        file->write( reply->readAll() );
}

void HttpDownload::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    emit dataReceived(bytesRead, totalBytes);
}

void HttpDownload::httpDownloadFinished()
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error()) // Means connection closed cause of error
    {
        emit downloadFinished(2, reply->errorString());
        return;
    }
    else if (!redirectionTarget.isNull()) // Means url redirected
    {
        if( redirectionCounter>5 )
            return;
        url = url.resolved(redirectionTarget.toUrl());
        reply->deleteLater();
        redirectionCounter++;
        qDebug() << redirectionCounter << ". time redirecting to " + url.toString();
        startRequest(url);
        return;
    }
    else // it means everything is done. You can set some variables from here
    {
        redirectionCounter = 0;
    }

    // Reset everything
    reply->deleteLater();
    reply =0;
    delete file;
    file =0;
    manager =0;
    emit downloadFinished(0, "");
}

void HttpDownload::download(QString requestedUrl, QString fileName, QString dirName)
{
    qDebug() << "Requesting "+requestedUrl;
    // Check internet connection
    if( isConnectedToInternet() )
    {
        // Start to request
        emit downloadStarted();
        startRequest(QUrl(requestedUrl));
    }
    else
    {
        emit downloadFinished(1, "Error 1 : No internet connection.");
        return;
    }

    // Handle download directory
    if( !QDir(dirName).exists() )
    {
        QDir().mkdir(dirName);
    }

    // Handle file name
    if( fileName.isEmpty() )
    {
        // Get the name from the url
        QFileInfo fileInfo(requestedUrl);
        fileName = fileInfo.fileName();

        if( fileName.isEmpty() )
            fileName = "faunus-market-unnamed.txt";
    }

    // Handle full file name with location
    fullFileName = dirName + "/" + fileName;

    // Create file
    file = new QFile(fullFileName);
    if( !file->open(QIODevice::WriteOnly) )
    {
        qDebug() << "Unable to write data to "+fullFileName;
        return;
    }
}

void HttpDownload::startRequest(QUrl requestedUrl)
{
    url = requestedUrl;

    // manager of our http works
    manager = new QNetworkAccessManager(this);

    // post request with get method then return QNetworkReply object
    // to read content and whenever new data is arrived
    // emit readyRead signal
    reply = manager->get(QNetworkRequest(url));

    // when reply emit readyRead signal, new data is received
    // so handle the things (like writing new data to file)
    connect(reply, SIGNAL(readyRead() ),
            this, SLOT(httpReadyRead() )
            );

    // when reply emit downloadProgress signal, new data is received
    // so handle the new data from updateDownloadProgress function
    connect(reply, SIGNAL(downloadProgress(qint64, qint64) ),
            this, SLOT(updateDownloadProgress(qint64, qint64) )
            );

    // When reply emit finished signal, run httpDownloadFinished function
    connect(reply, SIGNAL( finished() ),
            this, SLOT( httpDownloadFinished() )
            );
}

bool HttpDownload::isConnectedToInternet()
{
    // TODO: Put timeout. Its waiting too much at loop.exec
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply *replyTmp = nam.get(req);
    QEventLoop loop;
    connect(replyTmp, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(replyTmp->bytesAvailable())
        return true;
    else
        return false;
}
