#pragma once
#include "./capPacket.h"
#include <QMainWindow>
#include <string>
#include <QTableWidget>
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
