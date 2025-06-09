#include "http_request.hpp"
#include <sstream>

std::optional<HttpRequest> HttpRequest::parse(const std::string& request_data) {
    // Read the request line
    std::istringstream request_stream(request_data);
    std::string request_line;
    
    if ( !std::getline(request_stream, request_line) ) {
        return std::nullopt;
    }

    // Strip trailing carriage return
    if ( !request_line.empty() && request_line.back() == '\r' ) {
        request_line.pop_back();
    }
    
    // Split request line into tokens
    std::istringstream line_stream(request_line);
    std::string method, path, version;

    if ( !(((line_stream >> method) >> path) >> version) ) {
        return std::nullopt;
    }

    return HttpRequest{ method, path, version };
}