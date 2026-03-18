#pragma once
#include <stdio.h>
#include <string>
#include <pcap/pcap.h>

#pragma pack(push, 1)
struct ST_INFO
{
    // Len
    uint64_t len;
    // Type
    char type[16];
    // Sip
    char sip[16];
    // Dip
    char dip[16];
    // check code
    uint64_t chk_code;
};
#pragma pack(pop)

class Pcap
{
public:
    Pcap();
    ~Pcap();

    void runCap(const std::string& dev);
    void stopCap();

private:
    std::string device;
    bool isRunning;
    pcap_t* handle;
    char errbuf[PCAP_ERRBUF_SIZE];
};
