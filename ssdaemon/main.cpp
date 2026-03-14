#include "../capPacket.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("No Device Detected");
        fflush(stdout);
        return 1;
    }

    std::string devName = argv[1];

    Pcap worker;
    worker.runCap(devName);

    return 0;
}
