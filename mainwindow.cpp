#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "./device.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isRunning(false)
{
    ui->setupUi(this);
    this->setWindowTitle("Suseong Pcap");
    //this->setFixedSize(400, 600);
    connect(ui->runBtn, &QPushButton::clicked, this, &MainWindow::onStartButton);

    ui->devIn->clear();
    std::vector<std::string> devs = Device::getInstance().getDevices();
    std::vector<std::string>::iterator it = devs.begin();

    while(it != devs.end())
    {
        ui->devIn->addItem(QString::fromStdString(*it));
        it++;
    }

    #ifdef Q_OS_ANDROID
    int resSu = std::system("su -c 'echo root_check'");
    if(resSu != 0) {return;}
    #endif
    QStringList headers;
    headers << "Len" << "Type" << "SourceIp" << "DestinationIp";
    ui->packetTable->setColumnCount(4);
    ui->packetTable->setHorizontalHeaderLabels(headers);
    ui->packetTable->setColumnWidth(0, 45);
    ui->packetTable->setColumnWidth(1, 45);
    ui->packetTable->setColumnWidth(2, 125);
    ui->packetTable->setColumnWidth(3, 125);
    ui->packetTable->verticalHeader()->setVisible(false);

    pcapWorker = new Pcap(this);
    connect(pcapWorker, &Pcap::capPacket, this, &MainWindow::onReceivePacket, Qt::QueuedConnection);

    connect(pcapWorker, &Pcap::errorBox, this, &MainWindow::onError, Qt::QueuedConnection);
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
        pcapWorker->stopCap();
    }
}

void MainWindow::onError(QString msg)
{
    QMessageBox::critical(this, "Pcap open error", msg);

    if(isRunning == true)
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
