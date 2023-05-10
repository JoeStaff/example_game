#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

class Log {
public:
    Log(LogLevel level) : logLevel(level) {}

    template<typename T>
    Log& operator<<(const T& value) {
        if (logLevel >= currentLogLevel()) {
            std::cout << value;
        }
        return *this;
    }

    ~Log() {
        if (logLevel >= currentLogLevel()) {
            std::cout << std::endl;
        }
    }

private:
    static LogLevel currentLogLevel() {
#ifdef DEBUG
        return LogLevel::INFO;
#else
        return LogLevel::WARNING;
#endif
    }

    LogLevel logLevel;
};

#define log(level) Log(level)

#endif // LOGGER_H