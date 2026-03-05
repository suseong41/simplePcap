#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRunning(false)
{
    ui->setupUi(this);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onStartButton);

    QStringList headers;
    headers << "Len" << "Type" << "SourceIp" << "DestinationIp";
    ui->packetTable->setColumnCount(4);
    ui->packetTable->setHorizontalHeaderLabels(headers);

    ui->packetTable->setColumnWidth(0, 80);
    ui->packetTable->setColumnWidth(1, 80);
    ui->packetTable->setColumnWidth(2, 150);
    ui->packetTable->setColumnWidth(3, 150);

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
        pcapWorker->runCap(devType);
    }
    else
    {
        isRunning = false;
        ui->runBtn->setText("Start");
    }
}

void MainWindow::onReceivePacket(QString len, QString type, QString sip, QString dip)
{
    int row = ui->packetTable->rowCount();
    ui->packetTable->insertRow(row);

    ui->packetTable->setItem(row, 0, new QTableWidgetItem(len));
    ui->packetTable->setItem(row, 1, new QTableWidgetItem(type));
    ui->packetTable->setItem(row, 2, new QTableWidgetItem(sip));
    ui->packetTable->setItem(row, 3, new QTableWidgetItem(dip));
}
