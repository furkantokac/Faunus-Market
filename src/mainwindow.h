#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <httpdownload.h>
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
    void on_btnInstall_clicked();
    void on_btnRefresh_clicked();

    void finishDownload(bool error);
    void startDownload();
    void updateDownloadProgress(qint64 bytesRead, qint64 totalBytes);
    void errorDownload(int errorCode);

private:
    Ui::MainWindow *ui;

    bool refreshAppList = true;

    void loadTreeWidget(QTreeWidget *tw, QFile *file);
    QJsonObject jsonToObject(QFile *f);

    void apptop(QTextEdit *txe, QString text);

    QProgressBar *downBar;
    QTextEdit *txeStatus;

    HttpDownload downloader;
    const QString downloadDir = "cache";
    QStringList allSoftwares;

    void uiSetEnabled(bool state);
};

#endif // MAINWINDOW_H
