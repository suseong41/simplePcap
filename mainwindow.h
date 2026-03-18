#pragma once
#include "./capPacket.h"
#include <QMainWindow>
#include <pcap/pcap.h>
#include <string>
#include <QTableWidget>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>
#include <QScroller>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onStartButton();
    void onReceivePacket(QString len, QString type, QString sip, QString dip);
    void onError(QString msg);
    void onDaemonOutput();
    void onDaemonError();

private:
    Ui::MainWindow *ui;
    std::string devType;

    bool isRunning;
    QProcess *daemonProcess;

    QByteArray packetBuffer;
};

static QString dropPcapDaemon();
