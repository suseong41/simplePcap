#pragma once
#include <pcap/pcap.h>
#include <string>
#include <vector>

class Device
{
public:
    static Device& getInstance();
    std::vector<std::string> getDevices();
private:
    Device();
    ~Device();
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
};
