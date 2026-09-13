#include "../../inc/logger/Logger.h"
#include <chrono>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <exception>

std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);

    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

Logger::Logger()
{
    _configFilePath = "";
    _logLevels = std::unordered_map<std::string, LOG_LEVEL>();
    _appenders = std::unordered_map<std::string, Appender*>();
}

Logger *Logger::_pLoggerSingleton = nullptr;

const std::string Logger::LOGGER_KEYWORD = "LOGGER";
const std::string Logger::APPENDER_TYPE = "AppenderType";
const std::string Logger::APPENDER_TYPE_CONSOLE = "Console";
const std::string Logger::APPENDER_TYPE_FILE = "File";
const std::string Logger::APPENDER_FLUSH = "flush";

Logger *Logger::getInstance()
{
    if (_pLoggerSingleton == nullptr)
    {
        _pLoggerSingleton = new Logger();
    }
    return _pLoggerSingleton;
}

Logger::~Logger()
{
    for (auto const& appender : _appenders)
    {
        if (appender.second != nullptr)
        {
            delete(appender.second);
        }
    }
}

void Logger::setLogConfigFile(std::string config_file_path)
{
    std::printf("Adding logger");
    this->_configFilePath = config_file_path;
    parseConfigFile();
}

/* 
!CONFIG FILE FORMAT EXAMPLE

LOGGER.TestLog=INFO,MyAppender
LOGGER.AnotherTest=TRACE,MyAppender

MyAppender.AppenderType=Console
MyAppender.INFO=32
MyAppender.DEBUG=34
MyAppender.TRACE=32
MyAppender.WARN=35
MyAppender.ERROR=31;1;4
MyAppender.flush=true

!CONFIG FILE FORMAT EXAMPLE
*/

void Logger::parseConfigFile()
{
    std::cout << "Reading file: " << _configFilePath << std::endl;

    std::ifstream configFileStream = std::ifstream(_configFilePath);
    if (!configFileStream.good())
    {
        std::string errorMsg = "Failed to open file stream for: " + _configFilePath;
        throw std::runtime_error(errorMsg);
    }

    for (std::string line; std::getline(configFileStream, line);)
    {
        // Checks if line contains LOGGERs
        if (line.find(LOGGER_KEYWORD) != std::string::npos) //? Line that defines the visibility of a logger
        {
            parseLoggerConfig(line);
        }
        else if (!line.empty())
        {
            std::cout << line << std::endl;
            for (auto const& appender : _appenders)
            {
                if (appender.second != nullptr && line.find(appender.second->name) != std::string::npos)
                {
                    parseAppenderConfig(appender.second, line);
                    break;
                }
            }
        }
    }
}

void Logger::parseLoggerConfig(std::string line) //? line = 'LOGGER.TestLog=INFO,MyAppender'
{
    int dotPos;
    if ((dotPos = line.find('.')) == std::string::npos)
    {
        std::string errorMsg = "Couldn't parse '.' seperator in line: " + line;
        throw std::runtime_error(errorMsg);
    }

    std::string loggerName = line.substr(dotPos + 1); // After this step, loggerName = 'TestLog=INFO,MyAppender'

    int equalPos;
    if ((equalPos = loggerName.find('=')) == std::string::npos)
    {
        std::string errorMsg = "Couldn't parse '=' seperator in line: " + line;
        throw std::runtime_error(errorMsg);
    }

    std::string loggerOptions = loggerName.substr(equalPos + 1); // After this step, loggerOptions = 'INFO,MyAppender'
    loggerName = loggerName.substr(0, equalPos); // After this step, loggerName = 'TestLog'

    int commaPos;
    if ((commaPos = loggerOptions.find(',')) == std::string::npos)
    {
        std::string errorMsg = "Couldn't parse ',' seperator in line: " + line;
        throw std::runtime_error(errorMsg);
    }

    std::string appenderName = trim(loggerOptions.substr(commaPos + 1)); // After this step, appenderName = 'MyAppender'
    std::string loggerLevel = trim(loggerOptions.substr(0, commaPos)); // After this step, loggerLevel = 'INFO'
    
    _logLevels[loggerName] = LOG_LEVEL_HELPER::fromString(loggerLevel);
    std::cout << "adding appender:" << appenderName << std::endl;
    _appenders[loggerName] = new Appender(appenderName);
}

// needs to be called with enough info the construct an appender.
void Logger::parseAppenderConfig(Appender* pAppender, std::string line) //? line = 'MyAppender.AppenderType=Console'
{
    int dotPos;
    if ((dotPos = line.find('.')) == std::string::npos)
    {
        std::string errorMsg = "Couldn't parse '.' seperator in line: " + line;
        throw std::runtime_error(errorMsg);
    }

    std::string appenderOptionKey = line.substr(dotPos + 1); // After this step, appenderOptionKey = 'AppenderType=Console'
    
    int equalPos;
    if ((equalPos = appenderOptionKey.find('=')) == std::string::npos)
    {
        std::string errorMsg = "Couldn't parse '=' seperator in line: " + line;
        throw std::runtime_error(errorMsg);
    }

    std::string appenderOptionValue = trim(appenderOptionKey.substr(equalPos + 1)); // After this step, appenderOptionValue = 'Console'
    appenderOptionKey = trim(appenderOptionKey.substr(0, equalPos)); // After this step, appenderOptionKey = 'AppenderType'

    // after parsing the key & value we need to update the correct option 
    if (LOG_LEVEL_HELPER::fromString(appenderOptionKey) != LOG_LEVEL::NONE)
    {
        pAppender->addColorOption(LOG_LEVEL_HELPER::fromString(appenderOptionKey), appenderOptionValue);
    }
    else if (appenderOptionKey == APPENDER_TYPE)
    {
        if (appenderOptionValue != APPENDER_TYPE_CONSOLE && appenderOptionValue != APPENDER_TYPE_FILE)
        {
            std::string errorMsg = "Invalid appender type found in line: " + line;
            throw std::runtime_error(errorMsg);
        }
        pAppender->setIsConsole(appenderOptionValue == APPENDER_TYPE_CONSOLE);
    }
    else if (appenderOptionKey == APPENDER_FLUSH)
    {
        pAppender->setFlush(appenderOptionValue == "true");
    }
    else
    {
        std::string errorMsg = "Invalid appender option \"" + appenderOptionKey + "\" found in line: " + line;
        throw std::runtime_error(errorMsg);
    }
}



std::string Logger::getDateAndTime()
{
    std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
    std::time_t formated_time = std::chrono::system_clock::to_time_t(time);
    std::string time_to_print = std::ctime(&formated_time);
    time_to_print.pop_back();
    return "[" + time_to_print + "]";
}

void Logger::logToConsole(std::string color, std::string date, std::string logger, std::string msg)
{
    // std::cout << date << color << logger << ": " << msg << "\033[0m" << std::endl;
}

void Logger::logToFile(int fd, std::string date, std::string logger, std::string msg)
{
}

void Logger::logIfLevel(LOG_LEVEL level_required, std::string logger, std::string msg)
{
    // Logger* pLogger = Logger::getInstance();
    // if (pLogger->log_levels.count(logger) && (pLogger->log_levels[logger] >= level_required))
    // {
    //     // std::string color_settings = "\033[" + pLogger->appenders[pLogger->logger_appender_link[logger]].info_color + "\033[0m";
    //     // pLogger->log_colored(color_settings, getDateAndTime(), logger, msg);
    // }
}