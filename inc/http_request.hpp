#pragma once

#include <string>
#include <optional>

class HttpRequest {
    public:
        std::string method;
        std::string path;
        std::string version;

        static std::optional<HttpRequest> parse(const std::string& request_data);
};
