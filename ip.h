#pragma once
#include <pcap/pcap.h>

#pragma pack(push, 1)
struct ST_IP
{
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t total_len;
    uint16_t identifier;
    uint16_t flags_offset;
    uint8_t ttl;
    uint8_t proto_id;
    uint16_t checksum;
    uint8_t sip[4];
    uint8_t dip[4];
    uint32_t opt_padding;
};
#pragma pack(pop)
