#include "../../inc/logger/Logger.h"
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <sstream>

Logger::Logger()
{
    log_levels = std::unordered_map<std::string, Logger::LOG_LEVEL>();
    logger_appender_link = std::unordered_map<std::string, std::string>();
    appenders = std::unordered_map<std::string, Appender>();
}

void Logger::setLogConfigFile(std::string filePath)
{
    std::printf("Adding logger");
    readConfigFile(filePath);
}

void Logger::addAppender(std::string logger_name, std::string appender_name)
{
    Logger::logger_appender_link[logger_name] = appender_name;
}

void Logger::addLogger(std::string logger_name, std::string log_levels)
{
    if (log_levels == "INFO")
    {
        Logger::log_levels[logger_name] = LOG_LEVEL::INFO;
    }
    else if (log_levels == "DEBUG")
    {
        Logger::log_levels[logger_name] = LOG_LEVEL::DEBUG;
    }
    else if (log_levels == "TRACE")
    {
        Logger::log_levels[logger_name] = LOG_LEVEL::TRACE;
    }
    else if (log_levels == "WARN")
    {
        Logger::log_levels[logger_name] = LOG_LEVEL::WARN;
    }
    else if (log_levels == "ERROR")
    {
        Logger::log_levels[logger_name] = LOG_LEVEL::ERROR;
    }
}

/* 
!CONFIG FILE FORMAT EXAMPLE

LOGGER.TestLog=INFO,MyAppender
LOGGER.AnotherTest=TRACE,MyAppender

MyAppender.AppenderType=Console
MyAppender.info_color=32
MyAppender.debug_color=34
MyAppender.trace_color=32
MyAppender.warn_color=35
MyAppender.error_color=31;1;4
MyAppender.flush=true

!CONFIG FILE FORMAT EXAMPLE
*/


void Logger::readConfigFile(std::string filePath)
{
    std::ifstream config_file = std::ifstream(filePath);
    for (std::string line; std::getline(config_file, line);)
    {
        std::cout << line << std::endl;
        if (line.size() > 6 && line.substr(0, 6) == "LOGGER") //? Line that defines the visibility of a logger
        {
            readLoggerSettings(line);
        }
        else if (!line.empty())
        {
            readAppenderSettings(line);
        }
    }
}

// needs to be called with enough info the construct an appender.
void Logger::readAppenderSettings(std::string line)
{
    std::string appender_name = line.substr(0, line.find('.'));
    std::cout << "name of the appender to add: " << appender_name << std::endl;
    if (Logger::appenders.count(appender_name) == 0)
    {
        Logger::appenders[appender_name] = Appender();
        std::cout << "Created the AppenderInfo object for appender: " << appender_name << std::endl;;
    }
    
    std::string appender_info_tag = line.substr(appender_name.size() + 1, line.find('=') - appender_name.size() - 1);
    std::string appender_info_value = line.substr(line.find('=') + 1);

    std::cout << "====" << line.find('=') << " raw line is[" << line << "] size: " << line.size() << std::endl << std::endl;

    if (appender_info_tag == "AppenderType")
        Logger::appenders[appender_name].fileDescriptor = 1;
    else if (appender_info_tag == "info_color")
        Logger::appenders[appender_name].appender_color[LOG_LEVEL::INFO] = appender_info_value;
    else if (appender_info_tag == "debug_color")
        Logger::appenders[appender_name].appender_color[LOG_LEVEL::DEBUG] = appender_info_value;
    else if (appender_info_tag == "trace_color")
        Logger::appenders[appender_name].appender_color[LOG_LEVEL::TRACE] = appender_info_value;
    else if (appender_info_tag == "warn_color")
        Logger::appenders[appender_name].appender_color[LOG_LEVEL::WARN] = appender_info_value;
    else if (appender_info_tag == "error_color")
        Logger::appenders[appender_name].appender_color[LOG_LEVEL::ERROR] = appender_info_value;
    else if (appender_info_tag == "flush")
        if (appender_info_value == "true")
            Logger::appenders[appender_name].flush = true;
        else
            Logger::appenders[appender_name].flush = false;
}

void Logger::readLoggerSettings(std::string line) //? line = LOGGER.TestLog=INFO,MyAppender
{
    std::string logger = line.substr(7); //? logger = TestLog=INFO,MyAppender
    std::string logger_name = logger.substr(0, logger.find('=')); //? logger_name = TestLog


    std::istringstream config(logger.substr(logger_name.size() + 1)); //? config = INFO,MyAppender
    std::string config_info;
    std::getline(config, config_info, ','); //? config_info = INFO
    addLogger(logger_name, config_info);
    std::getline(config, config_info, ','); //? config_info = MyAppender
    addAppender(logger_name, config_info);
}

std::string Logger::getDateAndTime()
{
    auto time = std::chrono::system_clock::now();
    std::time_t formated_time = std::chrono::system_clock::to_time_t(time);
    std::string time_to_print = std::ctime(&formated_time);
    time_to_print.pop_back();
    return "[" + time_to_print + "]";
}

void Logger::log_colored(std::string color, std::string date, std::string logger, std::string msg)
{
    std::cout << date << color << logger << ": " << msg << "\033[0m" << std::endl;
}

void Logger::log_to_file(int fd, std::string date, std::string logger, std::string msg)
{
}

void Logger::log_if_level(LOG_LEVEL level_required, std::string logger, std::string msg)
{
    if (Logger::log_levels.count(logger) && (Logger::log_levels[logger] >= level_required))
    {
        std::string color_settings = "\033[" + Logger::appenders[Logger::logger_appender_link[logger]].info_color + "\033[0m";
        Logger::log_colored(color_settings, getDateAndTime(), logger, msg);
    }
}

void Logger::log_if_exist(std::string logger, std::string msg)
{
    if (Logger::log_levels.count(logger))
    {
        
    }
}
