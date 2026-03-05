#pragma once
#include "./capPacket.h"
#include <QMainWindow>
#include <pcap/pcap.h>
#include <string>
#include <QTableWidget>

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
public slots:
    void onStartButton();
    void onReceivePacket(QString len, QString type, QString sip, QString dip);

private:
    Ui::MainWindow *ui;
    std::string devType;

    bool isRunning;
    Pcap *pcapWorker;
};
