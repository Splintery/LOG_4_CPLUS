#ifndef LOGGER_H
#define LOGGER_H

#include <fcntl.h>
#include <string>
#include <unordered_map>
#include <unistd.h>

#include "LogLevel.hpp"
#include "Appender.h"

/*
\033[X;X;XmMymessage\033[0m
         foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47

Additionally, you can use these:

reset             0  (everything back to normal)
bold/bright       1  (often a brighter shade of the same colour)
underline         4
inverse           7  (swap foreground and background colours)
bold/bright off  21
underline off    24
inverse off      27
*/
#define LOG_INFO(name, msg) Logger::logIfLevel(LOG_LEVEL::INFO, name, msg)
#define LOG_DEBUG(name, msg) Logger::logIfLevel(LOG_LEVEL::DEBUG, name, msg)
#define LOG_TRACE(name, msg) Logger::logIfLevel(LOG_LEVEL::TRACE, name, msg)
#define LOG_WARN(name, msg) Logger::logIfExist(name, msg)
#define LOG_ERROR(name, msg) Logger::logIfExist(name, msg)


class Logger
{
public:
    /// @brief 
    /// @return 
    static Logger* getInstance();

    /// @brief 
    virtual ~Logger();

    /// @brief 
    /// @param  
    Logger(Logger &) = delete;

    /// @brief 
    /// @param  
    void setLogConfigFile(std::string);
private:
    static Logger* _pLoggerSingleton;
    Logger();

    std::string _configFilePath;
    // Links the name off a logger to a log level, the name of the log is used when logging
    std::unordered_map<std::string, LOG_LEVEL> _logLevels;
    // std::unordered_map<std::string, std::string> logger_appender_link;

    // Links the name of a logger to an appender
    std::unordered_map<std::string, Appender*> _appenders;
    
    void parseConfigFile();
    void parseLoggerConfig(std::string);
    void parseAppenderConfig(Appender*, std::string);

    static std::string getDateAndTime();

public:
    static const std::string LOGGER_KEYWORD;
    static const std::string APPENDER_TYPE;
    static const std::string APPENDER_TYPE_CONSOLE;
    static const std::string APPENDER_TYPE_FILE;
    static const std::string APPENDER_FLUSH;

    /// @brief 
    /// @param color 
    /// @param date 
    /// @param logger 
    /// @param msg 
    static void logToConsole(
        std::string color, 
        std::string date, 
        std::string logger, 
        std::string msg
    );
    static void logToFile(
        int fd, 
        std::string date, 
        std::string logger, 
        std::string msg
    );
    static void logIfLevel(LOG_LEVEL, std::string, std::string);
};

#endif