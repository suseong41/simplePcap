#pragma once
#include <string>

#ifndef NDEBUG
    #define TRACE(msg) Sutrace::print(__FILE__, __LINE__, __func__, msg)
#else
    #define TRACE(msg)
#endif

class Sutrace
{
public:
    static void print(const char* file, int line, const char* func, const std::string& msg);
private:
};
