#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

#define RESET_COLOR "\033[0m"
#define ERROR_COLOR "\033[0;31m"
#define WARNING_COLOR "\033[0;33m"
#define INFO_COLOR "\033[0;32m"

enum LogType
{
    ERROR,
    WARNING,
    INFO
};

class Logger
{
private:
    void displayMsg(LogType type, const std::string message);
    std::string getPrefixMsg(LogType type);

public:
    Logger(/* args */);
    ~Logger();
    void info(const std::string message);
    void warning(const std::string message);
    void error(const std::string message);
};

#endif