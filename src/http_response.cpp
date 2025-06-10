#include "http_response.hpp"

#include <sstream>      // Response building

std::string HttpResponse::build(const std::string& status_line, 
                                const std::string& content_type, 
                                const std::string& body) {
    std::ostringstream response_stream;

    // Prepare status line, a couple headers, and the text body
    response_stream << status_line << "\r\n"
                    << "Content-Type: " << content_type << "\r\n"
                    << "Content-Length: " << body.length() << "\r\n"
                    << "\r\n"
                    << body << "\r\n";

    return response_stream.str();
}

std::string HttpResponse::build_binary(const std::string& status_line,
                                const std::string& content_type,
                                const std::string& binary_body) {
    std::ostringstream headers;

    headers << status_line << "\r\n"
            << "Content-Type: " << content_type << "\r\n"
            << "Content-Length: " << binary_body.size() << "\r\n"
            << "\r\n";

    std::string response = headers.str();
    response.append(binary_body);

    return response;
}