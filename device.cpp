#include "device.h"

Device& Device::getInstance()
{
    static Device instance;
    return instance;
}

Device::Device(){}
Device::~Device(){}

std::vector<std::string> Device::getDevices()
{
    std::vector<std::string> devList;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldev;
    pcap_if_t* dev;

    if(pcap_findalldevs(&alldev, errbuf) == -1)
    {
        return devList;
    }

    dev = alldev;
    while(dev != nullptr)
    {
        if(dev->name != nullptr)
        {
            devList.push_back(std::string(dev->name));
        }
        dev = dev->next;
    }

    pcap_freealldevs(alldev);
    return devList;
}
