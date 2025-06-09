#include "logger.hpp"
#include <iostream>

namespace {
    // ANSI escape codes for coloured logging
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string CYAN    = "\033[36m";
}

namespace Logger {
    void info(const std::string& message) {
        std::cout << GREEN << "[INFO] " << message << RESET << std::endl;
    }

    void status(const std::string& message) {
        std::cout << CYAN << "[STATUS] " << message << RESET << std::endl;
    }

    void error(const std::string& message) {
        std::cerr << RED << "[ERROR] " << message << RESET << std::endl;
    }

    void warn(const std::string& message) {
        std::cerr << YELLOW << "[WARN] " << message << RESET << std::endl;
    }
}