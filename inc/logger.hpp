#pragma once

#include <string>

namespace Logger {
    std::string time_stamp();

    void info(const std::string& message);
    void status(const std::string& message);    // Request/response status
    void error(const std::string& message);
    void warn(const std::string& message);
}
