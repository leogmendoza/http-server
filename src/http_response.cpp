#include "http_response.hpp"
#include <sstream>

std::string HttpResponse::build(const std::string& status_line, 
                                const std::string& content_type, 
                                const std::string& body) {
    std::ostringstream response_stream;

    // Prepare status line, a couple headers, and the text body
    response_stream << status_line << "\r\n"
                    << "Content-Type: " << content_type << "\r\n"
                    << "Content-Length: " << body.length() << "\r\n"
                    << "\r\n"
                    << body << "\r\n"
                    << "";

    return response_stream.str();
}