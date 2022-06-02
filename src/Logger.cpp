#include "../include/Logger.hpp"

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::error(const std::string message)
{
    this->displayMsg(ERROR, message);
}

void Logger::warning(const std::string message)
{
    this->displayMsg(WARNING, message);
}

void Logger::info(const std::string message)
{
    this->displayMsg(INFO, message);
}

void Logger::displayMsg(LogType type, const std::string message)
{
    std::string prefix = this->getPrefixMsg(type);

    std::cout << prefix + " " + message << RESET_COLOR << std::endl;
}

std::string Logger::getPrefixMsg(LogType type)
{
    if (type == ERROR)
    {
        return ERROR_COLOR "[ERROR]";
    }
    else if (type == WARNING)
    {
        return WARNING_COLOR "\033[1m\033[33m[WARNING]";
    }
    else if (type == INFO)
    {
        return INFO_COLOR "\033[32m[INFO]";
    }

    return RESET_COLOR;
}