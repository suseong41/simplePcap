#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRunning(false)
{
    ui->setupUi(this);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onStartButton);

    lenModel = new QStringListModel(this);
    ui->lenView->setModel(lenModel);

    typeModel = new QStringListModel(this);
    ui->typeView->setModel(typeModel);

    saModel = new QStringListModel(this);
    ui->saView->setModel(saModel);

    daModel = new QStringListModel(this);
    ui->daView->setModel(daModel);

    pcapWorker = new Pcap(this);
    connect(pcapWorker, &Pcap::capPacket, this, &MainWindow::onReceivePacket);
}

MainWindow::~MainWindow()
{
    pcapWorker->stopCap();
    delete ui;
}

void MainWindow::onStartButton()
{
    if(isRunning == false)
    {
        isRunning = true;
        ui->runBtn->setText("Stop");

        devType = ui->devIn->text().toStdString();
    }
    else
    {
        isRunning = false;
        ui->runBtn->setText("Start");
    }
}

void MainWindow::onReceivePacket(QString len, QString type, QString sip, QString dip)
{
    lenList.append(len);
    lenModel->setStringList(lenList);

    typeList.append(type);
    typeModel->setStringList(typeList);

    saList.append(sip);
    saModel->setStringList(saList);

    daList.append(dip);
    daModel->setStringList(daList);
}
