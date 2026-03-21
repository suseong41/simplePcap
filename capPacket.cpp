#include "capPacket.h"
#include "eth.h"
#include "ip.h"
#include "./sutrace.h"

Pcap::Pcap()
{
    isRunning = false;
    pcap = nullptr;
}

Pcap::~Pcap()
{
    stopCap();
}

void Pcap::runCap(const std::string& dev)
{
    device = dev;
    isRunning = true;

    pcap = pcap_open_live(device.c_str(), BUFSIZ, 1, 1, errbuf);
    if(pcap == nullptr)
    {
        std::string errMsg = "pcap_opne_live failed: ";
        errMsg += errbuf;
        TRACE(errMsg);
        return;
    }

    struct pcap_pkthdr* header;
    const u_char* packet;
    int res;

    while(isRunning == true)
    {
        res = pcap_next_ex(pcap, &header, &packet);
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

        ST_INFO info = {0};
        info.len = header->len;
        info.chk_code = 0xABCD1234;

        std::string typeStr = "UNKNOWN";
        std::string sipStr = " ";
        std::string dipStr = " ";

        if (etherType!=0x0800)
        {
            if (etherType == 0x0806) typeStr = "ARP";
            snprintf(info.type, sizeof(info.type), "%s", typeStr.c_str());
            fwrite(&info, sizeof(ST_INFO), 1, stdout);
            fflush(stdout);
            continue;
        }
        ST_IP* ip = (ST_IP*)(packet+sizeof(ST_ETH));
        uint8_t protoId = ip->proto_id;
        if (protoId == 1) typeStr = "ICMP";
        else if (protoId == 6) typeStr = "TCP";
        else if (protoId == 17) typeStr = "UDP";
        else typeStr = "IPv4";

        snprintf(info.type, sizeof(info.type), "%s", typeStr.c_str());
        snprintf(info.sip, sizeof(info.sip), "%d.%d.%d.%d", ip->sip[0], ip->sip[1], ip->sip[2], ip->sip[3]);
        snprintf(info.dip, sizeof(info.dip), "%d.%d.%d.%d", ip->dip[0], ip->dip[1], ip->dip[2], ip->dip[3]);
        //size_t fwrite(
        //    const void *buffer,
        //    size_t size,
        //    size_t count,
        //    FILE *stream
        //    );
        fwrite(&info, sizeof(ST_INFO), 1, stdout);
        fflush(stdout);

    }

    pcap_close(pcap);
    pcap = nullptr;
}

void Pcap::stopCap()
{
    isRunning = false;
    if(pcap != nullptr)
    {
        pcap_breakloop(pcap);
    }
}

