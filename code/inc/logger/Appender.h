#ifndef APPENDER_H
#define APPENDER_H

#include <string>
#include <fcntl.h>

#include "LogLevel.h"

// An appender defines where and how the msg is outputed (terminal/file)
class Appender
{
private:
    std::string appender_colors[5]; //* 0 is INFO, 1 is DEBUG, 2 is TRACE, 3 is WARN anc 4 is ERROR
    bool flush;

    virtual ~Appender();
    std::string formatMessage(LOG_LEVEL level, std::string message);
    
public:
    Appender(std::string info_color, std::string debug_color, std::string trace_color, std::string warn_color, std::string error_color, bool flush);
    virtual void printMessage(LOG_LEVEL level, std::string message);
};

#endif