#include "capPacket.h"
#include "eth.h"
#include "ip.h"
#include "./sutrace.h"

Pcap::Pcap()
{
    isRunning = false;
    handle = nullptr;
}

Pcap::~Pcap()
{
    stopCap();
}

void Pcap::runCap(const std::string& dev)
{
    device = dev;
    isRunning = true;

    handle = pcap_open_live(device.c_str(), BUFSIZ, 1, 1000, errbuf);
    if(handle == nullptr)
    {
        std::string errMsg = "pcap_opne_live failed: ";
        errMsg += errbuf;
        TRACE(errMsg);
        printf("ERROR|%s\n", errMsg.c_str());
        fflush(stdout);
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

        ST_ETH* eth = (ST_ETH*)packet;
        uint16_t etherType = htons(eth->len_type);
        std::string typeStr = "None";
        std::string sipStr = " ";
        std::string dipStr = " ";
        if (etherType!=0x0800)
        {
            if (etherType == 0x0806) typeStr = "ARP";
            printf("%d|%s|%s|%s\n", header->len, typeStr.c_str(), sipStr.c_str(), dipStr.c_str());
            fflush(stdout);
            continue;
        }
        ST_IP* ip = (ST_IP*)(packet+sizeof(ST_ETH));
        uint8_t protoId = ip->proto_id;
        if (protoId == 1) typeStr = "ICMP";
        else if (protoId == 6) typeStr = "TCP";
        else if (protoId == 17) typeStr = "UDP";
        else typeStr = "IPv4";

        char sipBuf[16];
        char dipBuf[16];
        snprintf(sipBuf, sizeof(sipBuf), "%d.%d.%d.%d", ip->sip[0], ip->sip[1], ip->sip[2], ip->sip[3]);
        snprintf(dipBuf, sizeof(dipBuf), "%d.%d.%d.%d", ip->dip[0], ip->dip[1], ip->dip[2], ip->dip[3]);
        printf("%d|%s|%s|%s\n", header->len, typeStr.c_str(), sipBuf, dipBuf);
        fflush(stdout);
    }

    pcap_close(handle);
    handle = nullptr;
}

void Pcap::stopCap()
{
    isRunning = false;
    if(handle != nullptr)
    {
        pcap_breakloop(handle);
    }
}

