#pragma once
#include <QObject>
#include <QString>
#include <string>
#include <thread>
#include <pcap/pcap.h>

class Pcap : public QObject
{
    Q_OBJECT
public:
    explicit Pcap(QObject *parent = nullptr);
    ~Pcap();

    void runCap(const std::string& dev);
    void stopCap();

signals:
    void capPacket(QString len, QString type, Qstirng sip, QString dip);
private:
    void run();
    std::string device;
    bool isRunning;
    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];

    std::thread* workerThread;
};
