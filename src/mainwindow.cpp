#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qInfo() << "napon" ;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
