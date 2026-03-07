#include "capPacket.h"
#include "eth.h"
#include "ip.h"

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
        ST_ETH* eth = (ST_ETH*)packet;
        uint16_t etherType = htons(eth->len_type);
        QString typeStr = "None";
        QString sipStr = " ";
        QString dipStr = " ";
        if (etherType!=0x0800)
        {
            if (etherType == 0x0806) typeStr = "ARP";
            emit capPacket(lenStr, typeStr, sipStr, dipStr);
            continue;
        }
        ST_IP* ip = (ST_IP*)(packet+sizeof(ST_ETH));
        uint8_t protoId = ip->proto_id;
        if (protoId == 1) typeStr = "ICMP";
        else if (protoId == 6) typeStr = "TCP";
        else if (protoId == 17) typeStr = "UDP";
        else typeStr = "IPv4";

        sipStr = QString::number(ip->sip[0])+"."+QString::number(ip->sip[1])+"."+QString::number(ip->sip[2])+"."+QString::number(ip->sip[3]);
        dipStr = QString::number(ip->dip[0])+"."+QString::number(ip->dip[1])+"."+QString::number(ip->dip[2])+"."+QString::number(ip->dip[3]);
        emit capPacket(lenStr, typeStr, sipStr, dipStr);
    }

    pcap_close(handle);
    handle = nullptr;
}
