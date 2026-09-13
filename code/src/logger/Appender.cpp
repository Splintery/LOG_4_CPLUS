#include "../../inc/logger/Appender.h"
#include <iostream>
#include <string>

Appender::Appender(std::string appenderName): name(appenderName) {}

Appender::~Appender()
{
}

void Appender::addColorOption(LOG_LEVEL level, std::string color)
{
    _appenderColors[(int)level] = color;
}

void Appender::setIsConsole(bool isConsoleAppender)
{
    _isConsoleAppender = isConsoleAppender;
}

void Appender::setFlush(bool flush)
{
    _flush = flush;
}

void Appender::printMessage(LOG_LEVEL level, std::string message)
{
    if (_isConsoleAppender)
    {
        message = formatMessage(level, message);
    }
    std::cout << message << std::endl;
}

std::string Appender::formatMessage(LOG_LEVEL level, std::string message)
{
    return "\033[" + _appenderColors[(int) level] + "m" + message + "\033]0m";
}