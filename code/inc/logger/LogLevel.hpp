#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

#include <string>

enum class LOG_LEVEL
{
    INFO = 0,
    DEBUG = 1,
    TRACE = 2,
    WARN = 3,
    ERROR = 4,
    NONE = 5
};

class LOG_LEVEL_HELPER
{
private:
    static const inline std::string enumStringFormat[6] = {"INFO", "DEBUG", "TRACE", "WARN", "ERROR", "NONE"};
    LOG_LEVEL_HELPER() = delete;
    ~LOG_LEVEL_HELPER() = delete;
public:
    static LOG_LEVEL fromString(std::string level)
    {
        for (int i = 0; i < 6; i++)
        {
            if (level == enumStringFormat[i])
            {
                return (LOG_LEVEL)i;
            }
        }
        return LOG_LEVEL::NONE;
    }

    static std::string toString(LOG_LEVEL level)
    {
        return enumStringFormat[(int)level];
    }
};


#endif