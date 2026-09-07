#include <cstdio>
#include "../inc/logger/Logger.h"

int main()
{
    std::printf("HElloWorld");
    Logger::getInstance().setLogConfigFile("loggerConfig.config");
}