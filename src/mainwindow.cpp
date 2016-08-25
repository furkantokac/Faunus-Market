#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    txeStatus = ui->textEditStatus;
    downBar = ui->progressBarDownload;
    downBar->setValue(0);

    // When reply emit finished signal, run httpDownloadFinished function
    connect(&downloader, SIGNAL( dataReceived(qint64,qint64)),
            this, SLOT( updateDownloadProgress(qint64,qint64) )
            );

    connect(&downloader, SIGNAL( downloadFinished(bool) ),
            this, SLOT( finishDownload(bool) )
            );

    connect(&downloader, SIGNAL( downloadStarted() ),
            this, SLOT( startDownload() )
            );

    connect(&downloader, SIGNAL(downloadError(int) ),
            this, SLOT( errorDownload(int) )
            );

    // Get the config file of Faunus Market from web
    refreshAppList = true;
    downloader.download("https://raw.github.com/furkantokac/Faunus-Config/master/faunus-market-config.json", "", "cache");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::errorDownload(int errorCode)
{
    switch(errorCode)
    {
    case 0:
        break;

    case 1:
        qDebug() << "No internet connection.";
    }
}

void MainWindow::finishDownload(bool error)
{
    uiSetEnabled(true);
    ui->progressBarDownload->setValue(0);
    if( error )
        qDebug() << "Download cancelled.";
    else
    {
        qDebug() << "Download finished successfuly.";
        if( refreshAppList )
        {
            refreshAppList=false;
            on_btnRefresh_clicked();
        }
    }
}

void MainWindow::startDownload()
{
    qDebug() << "Download started.";
    uiSetEnabled(false);
}

void MainWindow::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    downBar->setMaximum(totalBytes);
    downBar->setValue(bytesRead);
}

void MainWindow::loadTreeWidget(QTreeWidget *tw, QFile *f)
{
    if( !f->open(QIODevice::ReadOnly) )
    {
        qDebug() << "Unable to read json file.";
        return;
    }

    QJsonObject json = jsonToObject(f);
    allSoftwares = json["all-softwares"].toString().split(",");

    for(int i=0; i<allSoftwares.size(); i+=1)
    {
        QStringList theApp = json[ allSoftwares[i] ].toString().split(",");
        QTreeWidgetItem *softwares = new QTreeWidgetItem(tw);
        softwares->setText(0, theApp[0]);
        softwares->setText(1, theApp[1]);
        softwares->setText(2, theApp[2]);
        softwares->setText(3, theApp[3]);
    }
}

QJsonObject MainWindow::jsonToObject(QFile *f)
{
    QByteArray jsonData = f->readAll();
    QJsonDocument jsonFile = QJsonDocument::fromJson(jsonData);
    return jsonFile.object();
}

void MainWindow::apptop(QTextEdit *txe, QString text)
{
    QString oldText = txe->toPlainText();       // or toHtml()
    txe->setPlainText("-> "+text+ "\n" + oldText);    // or setText() or setHtml()
}

void MainWindow::uiSetEnabled(bool state)
{
    ui->btnInstall->setEnabled(state);
    ui->btnRefresh->setEnabled(state);
    ui->btnUninstall->setEnabled(state);
    ui->btnUpdate->setEnabled(state);
}

void MainWindow::on_btnInstall_clicked()
{
    QTreeWidgetItem *selectedItem = ui->treeWidgetApps->currentItem();
    if( !selectedItem ) // if there is no item selected, return
        return;
    QString selectedApp = selectedItem->data(0, 0).toString();
    qDebug() << selectedApp;
    downloader.download("https://github.com/furkantokac/"+selectedApp+"/archive/master.zip", selectedApp+".zip", "cache");
    apptop(txeStatus, "Downloading "+selectedApp);
    //https://github.com/furkantokac/Fchat/archive/master.zip
    //https://github.com/furkantokac/Fmail/archive/master.zip
}

void MainWindow::on_btnRefresh_clicked()
{
    if( QFile("cache/faunus-market-config.json").exists() )
    {
        ui->treeWidgetApps->clear();
        QFile file("cache/faunus-market-config.json");
        loadTreeWidget(ui->treeWidgetApps, &file);
    }
    else
        qDebug() << "faunus-market-config.json doesn't exist.";
}
