#pragma once
#include <pcap/pcap.h>
#include "mac.h"

#pragma pack(push, 1)
struct ST_ETH
{
    ST_MAC da;
    ST_MAC sa;
    uint16_t len_type;
};
#pragma pack(pop)
