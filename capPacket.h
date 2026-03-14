#pragma once
#include <string>
#include <pcap/pcap.h>

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
