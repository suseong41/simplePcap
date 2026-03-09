#include "sutrace.h"

void Sutrace::print(const char* file, int line, const char* func, const std::string& msg)
{
    std::string fileStr = file;
    size_t pos = fileStr.find_last_of("/\\");

    if(pos != std::string::npos)
    {
        fileStr = fileStr.substr(pos+1);
    }

    QString traceMsg = QString("[%1:%2] %3() - %4").arg(fileStr.c_str()).arg(line).arg(func).arg(msg.c_str());
    qDebug().noquote() << traceMsg;
}
