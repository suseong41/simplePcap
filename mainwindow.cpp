#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRunning(false)
{
    ui->setupUi(this);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onStartButton);

    ui->devIn->addItem("en0");
    ui->devIn->addItem("Wlan0");

    QStringList headers;
    headers << "Len" << "Type" << "SourceIp" << "DestinationIp";
    ui->packetTable->setColumnCount(4);
    ui->packetTable->setHorizontalHeaderLabels(headers);

    ui->packetTable->setColumnWidth(0, 60);
    ui->packetTable->setColumnWidth(1, 80);
    ui->packetTable->setColumnWidth(2, 135);
    ui->packetTable->setColumnWidth(3, 135);

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
        ui->packetTable->setRowCount(0);

        isRunning = true;
        ui->runBtn->setText("Stop");

        devType = ui->devIn->currentText().toStdString();
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

    QTableWidgetItem *itemLen = new QTableWidgetItem(len);
    QTableWidgetItem *itemType = new QTableWidgetItem(type);
    QTableWidgetItem *itemSip = new QTableWidgetItem(sip);
    QTableWidgetItem *itemDip = new QTableWidgetItem(dip);

    itemLen->setTextAlignment(Qt::AlignCenter);
    itemType->setTextAlignment(Qt::AlignCenter);

    ui->packetTable->setItem(row, 0, itemLen);
    ui->packetTable->setItem(row, 1, itemType);
    ui->packetTable->setItem(row, 2, itemSip);
    ui->packetTable->setItem(row, 3, itemDip);
}
