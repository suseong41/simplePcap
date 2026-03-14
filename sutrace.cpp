#include "sutrace.h"

void Sutrace::print(const char* file, int line, const char* func, const std::string& msg)
{
    std::string fileStr = file;
    size_t pos = fileStr.find_last_of("/\\");

    if(pos != std::string::npos)
    {
        fileStr = fileStr.substr(pos+1);
    }

    printf("[%s:%d] %s() - %s\n", fileStr.c_str(), line, func, msg.c_str());
    fflush(stdout);
}
