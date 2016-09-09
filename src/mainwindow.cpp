#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // When reply emit finished signal, run httpDownloadFinished function.
    connect(&downloader, SIGNAL( dataReceived(qint64,qint64)),
            this, SLOT( updateDownloadProgress(qint64,qint64) )
            );

    connect(&downloader, SIGNAL( downloadFinished(int,QString) ),
            this, SLOT( finishDownload(int,QString) )
            );

    connect(&downloader, SIGNAL( downloadStarted() ),
            this, SLOT( startDownload() )
            );

    ui->progressBarDownload->setValue(0);

    // Load config files
    if( appsConfig.load(appsConfigFile) )
        loadTreeWidget(ui->treeWidgetApps);
    else
        updateStatus("Applist couldn't find. Please refresh applist.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startDownload()
{

}

void MainWindow::finishDownload(int error, QString errorStr)
{
    ui->progressBarDownload->setValue(0);
    if( !error )
    {
        qDebug() << "Download finished successfuly.";
        switch( whatIsDownloaded )
        {
        case APPLIST:
            loadTreeWidget(ui->treeWidgetApps);
            updateStatus("Latest applist has successfully loaded.");
            break;

        case APP:
            updateStatus(downloadedAppName+" has successfully downloaded." );
            break;

        default:
            break;
        }
    }
    else
    {
        switch(error)
        {
        case 1:
            updateStatus(errorStr);
            break;

        case 2:
            updateStatus(errorStr);
            break;

        default:
            break;
        }
    }
    whatIsDownloaded=0;
}

void MainWindow::updateStatus(QString msg)
{
    apptop(ui->textEditStatus, "-> "+msg);
    //qDebug() << msg;
}

void MainWindow::updateDownloadProgress(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBarDownload->setMaximum(totalBytes);
    ui->progressBarDownload->setValue(bytesRead);
}

void MainWindow::apptop(QTextEdit *txe, QString text)
{
    QString oldText = txe->toPlainText();     // or toHtml()
    txe->setPlainText(text+ "\n" + oldText);  // or setText() or setHtml()
}

void MainWindow::uiSetEnabled(bool state)
{
    ui->btnInstallApp->setEnabled(state);
    ui->btnRefreshApplist->setEnabled(state);
    ui->btnUninstallApp->setEnabled(state);
    ui->btnUpdateApp->setEnabled(state);
}

void MainWindow::loadTreeWidget(QTreeWidget *tw)
{
    if( !appsConfig.load(appsConfigFile) )
    {
        updateStatus("Unable to read json file.");
        return;
    }

    QStringList allSoftwares = appsConfig.getStringList("all-softwares");

    // Clean current items in treeWidget before reload
    ui->treeWidgetApps->clear();

    for(int i=0; i<allSoftwares.size(); i+=1)
    {
        QStringList theApp = appsConfig.getStringList( allSoftwares[i] );
        QTreeWidgetItem *softwares = new QTreeWidgetItem(tw);
        softwares->setText(0, theApp[0]);
        softwares->setText(1, theApp[1]);
        softwares->setText(2, theApp[2]);
        softwares->setText(3, theApp[3]);
    }
}

void MainWindow::on_btnInstallApp_clicked()
{

}

void MainWindow::on_btnRefreshApplist_clicked()
{
    whatIsDownloaded = APPLIST;
    downloader.download(marketConfig.get("apps-config-url"), "", "cache");
    updateStatus("Downloading latest applist.");
}

void MainWindow::on_btnDownloadApp_clicked()
{
    QTreeWidgetItem *selectedItem = ui->treeWidgetApps->currentItem();
    if( !selectedItem ) // if there is no item selected, return
        return;
    QString selectedApp = selectedItem->data(0, 0).toString();
    whatIsDownloaded = APP;
    downloadedAppName = selectedApp;
    downloader.download(appsConfig.get(selectedApp, 5), selectedApp+".zip", "cache");
}
