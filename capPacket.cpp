#include "capPacket.h"

Pcap::Pcap(QObject *parent) : QObject(parent)
{
    isRunning = false;
    handle = nullptr;
    workerThread = nullptr;
}

Pcap::~Pcap()
{
    stopCap();
}

void Pcap::runCap(const std::string& dev)
{
    device = dev;
    isRunning = true;

    workerThread = new std::thread(&Pcap::run, this);
}

void Pcap::stopCap()
{
    isRunning = false;
    if(handle != nullptr)
    {
        pcap_breakloop(handle);
    }

    if(workerThread != nullptr)
    {
        if(workerThread->joinable())
        {
            workerThread->join();
        }
        delete workerThread;
        workerThread = nullptr;
    }
}

void Pcap::run()
{
    handle = pcap_open_live(device.c_str(), BUFSIZ, 1, 1000, errbuf);
    if(handle == nullptr)
    {
        fprintf(stderr, "pcap_open_live failed: %s\n", errbuf);
        return;
    }

    struct pcap_pkthdr* header;
    const u_char* packet;
    int res;

    while(isRunning == true)
    {
        res = pcap_next_ex(handle, &header, &packet);
        if(res == 0)
        {
            continue;
        }
        if(res == -1 || res == -2)
        {
            break;
        }
        QString lenStr = QString::number(header->len);
        emit capPacket(lenStr, "Type", "SIP", "DIP");
    }

    pcap_close(handle);
    handle = nullptr;
}
