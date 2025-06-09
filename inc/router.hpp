#pragma once

#include <string>
#include "http_request.hpp"

struct RouteResult {
    std::string status_line;
    std::string content_type;
    std::string body;
};

class Router {
    public:
        static RouteResult route(const HttpRequest& request);
};
