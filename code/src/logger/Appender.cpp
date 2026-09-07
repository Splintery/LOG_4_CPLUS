#include "../../inc/logger/Appender.h"
#include <iostream>
#include <string>

Appender::Appender(std::string info_color, std::string debug_color, std::string trace_color, std::string warn_color, std::string error_color, bool flush): flush{flush}
{
    appender_colors[0] = info_color;
    appender_colors[1] = debug_color;
    appender_colors[2] = trace_color;
    appender_colors[3] = warn_color;
    appender_colors[4] = error_color;
}

std::string Appender::formatMessage(LOG_LEVEL level, std::string message)
{
    return "\033[" + appender_colors[(int) level] + "m" + message + "\033]0m";
}

void Appender::printMessage(LOG_LEVEL level, std::string message)
{
    std::cout << formatMessage(level, message) << std::endl;
}