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
    std::vector<std::string> devs = Device::getInstance().getDevice();
    std::vector<std::string>::iterator it = devs.begin();

    while(it != devs.end())
    {
        ui->devIn->addItem(QString::fromStdString(*it));
        it++;
    }


    #ifdef Q_OS_ANDROID
    int resSu = std::system("su -c 'root chk'");
    if(resSu != 0) return;

    QString targetPath = dropPcapDaemon();
    if(targetPath != "")
    {
        daemonProcess = new QProcess(this);
        QStringList args;
        args << "-c" << "targetPath";
        connect(daemonProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::onDaemonOutput);
        daemonProcess->start("su", args);
    }
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

void MainWindow::onDaemonOutput()
{
    if(daemonProcess == nullptr) return;
    QByteArray output = daemonProcess->readAllStandardOutput();
    QString outputStr = QString::fromUtf8(output);

    qDebug() << "[Daemon] " << outputStr;
}

static QString dropPcapDaemon()
{
    // AppDataLocation -> /data/data/<패키지명>/files
    // QStandardPaths::writeableLocation -> 쓰기권한을 가지는 시스템 경로 QString
    QString targetDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(targetDir);

    if(!dir.exists())
    {
        // .은 현재 디렉터리
        dir.mkpath(".");
    }

    QString targetPath = targetDir + "/ssdaemon";
    QFile targetFile(targetPath);

    if(targetFile.exists())
    {
        // 기존 파일 발견시 제거
        targetFile.remove();
    }
    // assets에서 추출해옴
    QFile assetFile("assets:/ssdaemon");

    // chmod 755
    if(assetFile.copy(targetPath))
    {
        QFile::setPermissions(targetPath, QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner |
                                              QFileDevice::ReadGroup | QFileDevice::ExeGroup |
                                              QFileDevice::ReadOther | QFileDevice::ExeOther);
        return targetPath;
    }

    return QString("");
}
