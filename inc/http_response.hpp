#pragma once

#include <string>

class HttpResponse {
    public:
        static std::string build(const std::string& status_line,
                                 const std::string& content_type,
                                 const std::string& body);

        static std::string build_binary(const std::string& status_line,
                                        const std::string& content_type,
                                        const std::string& binary_body);                        
};
