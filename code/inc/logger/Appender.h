
#ifndef APPENDER_H
#define APPENDER_H

#include <string>
#include <fcntl.h>

#include "LogLevel.hpp"

// An appender defines where and how the msg is outputed (terminal/file)
class Appender
{
private:
    std::string _appenderColors[5]; //* 0 is INFO, 1 is DEBUG, 2 is TRACE, 3 is WARN anc 4 is ERROR
    bool _flush;
    bool _isConsoleAppender;

    std::string formatMessage(LOG_LEVEL level, std::string message);
    
public:
    const std::string name;
    Appender(std::string appenderName);
    virtual ~Appender();

    void addColorOption(LOG_LEVEL, std::string);
    void setIsConsole(bool);
    void setFlush(bool);

    virtual void printMessage(LOG_LEVEL level, std::string message);
};

#endif