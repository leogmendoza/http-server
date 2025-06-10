#include "logger.hpp"

#include <iostream>
#include <ctime>

namespace {
    // ANSI escape codes for coloured logging
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string CYAN    = "\033[36m";
}

namespace Logger {
    std::string time_stamp() {
        std::time_t current_time = std::time(nullptr);
        std::tm* local_time = std::localtime(&current_time);

        char buffer[10];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", local_time);

        return std::string("[") + buffer + "]";
    }

    void info(const std::string& message) {
        std::cout << GREEN << time_stamp() << " [INFO] " << message << RESET << std::endl;
    }

    void status(const std::string& message) {
        std::cout << CYAN << time_stamp() << " [STATUS] " << message << RESET << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << RED << time_stamp() << " [ERROR] " << message << RESET << std::endl;
    }

    void warn(const std::string& message) {
        std::cerr << YELLOW << time_stamp() << " [WARN] " << message << RESET << std::endl;
    }
}