#ifndef LOGGER_H
#define LOGGER_H

#include <fcntl.h>
#include <string>
#include <unordered_map>
#include <unistd.h>

#include "LogLevel.h"
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
#define LOG_INFO(name, msg) Logger::log_if_level(LOG_LEVEL::INFO, name, msg)
#define LOG_DEBUG(name, msg) Logger::log_if_level(LOG_LEVEL::DEBUG, name, msg)
#define LOG_TRACE(name, msg) Logger::log_if_level(LOG_LEVEL::TRACE, name, msg)
#define LOG_WARN(name, msg) Logger::log_if_exist(name, msg)
#define LOG_ERROR(name, msg) Logger::log_if_exist(name, msg)


class Logger
{
public:
    static Logger getInstance();
    virtual ~Logger();
    Logger(Logger &) = delete;
    void setLogConfigFile(std::string);

private:
    static Logger *_pLoggerSingleton;
    Logger();

    // Links the name off a logger to a log level, the name of the log is used when logging
    std::unordered_map<std::string, LOG_LEVEL> log_levels;
    // Links the name of a logger to an appender
    std::unordered_map<std::string, std::string> logger_appender_link;
    // 
    std::unordered_map<std::string, Appender> appenders;
    
    void addAppender(std::string, std::string);
    void addLogger(std::string, std::string);
    void readConfigFile(std::string);
    void readAppenderSettings(std::string);
    void readLoggerSettings(std::string);
    std::string getDateAndTime();

public:
    static void log_colored(
        std::string color, 
        std::string date, 
        std::string logger, 
        std::string msg
    );
    static void log_to_file(
        int fd, 
        std::string date, 
        std::string logger, 
        std::string msg
    );
    static void log_if_level(LOG_LEVEL, std::string, std::string);
    static void log_if_exist(std::string, std::string);
};

#endif