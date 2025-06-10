#include "router.hpp"

#include <sstream>          // Response building
#include <fstream>          // File reading
#include <filesystem>       // File checking

#include "http_response.hpp"

RouteResult Router::route(const HttpRequest& request) {
    RouteResult result;
    std::string file_path;

    result.status_line = "HTTP/1.1 200 OK";
    result.content_type = "text/plain";

    // Path handling -- TO DO: Move out / In general, split up handle_client
    if (request.path == "/") {
        // Note: Relative path accounts for building and executing within the `build` folder
        file_path = "../public/index.html";
    } else if (request.path == "/about") {
        file_path = "../public/about.html";
    } else if (request.path == "/heres_the_thing") {
        file_path = "../public/heres_the_thing.html";
    } else if (request.path == "/forbidden") {
        file_path = "../public/forbidden.html";
    } else if ( request.path.rfind("/images/", 0) == 0 ) {
        // Dynamically serve from public/images
        file_path = "../public" + request.path;

        // Check for the requested image
        if ( !std::filesystem::exists(file_path) ) {
            result.status_line = "HTTP/1.1 404 Not Found";
            result.content_type = "text/plain";
            result.body = "404 - Image not found";

            return result;    
        }

        // Read from png in binary mode to avoid corruption
        std::ifstream file(file_path, std::ios::binary);

        std::ostringstream buffer;
        buffer << file.rdbuf();

        result.status_line = "HTTP/1.1 200 OK";
        result.content_type = "image/png";
        result.body = buffer.str();

        return result;
    } else {
        result.status_line = "HTTP/1.1 404 Not Found";
        result.content_type = "text/plain";
        result.body = "404 - Ermm... unexpected path D:";
        
        return result;
    }

    std::ifstream file(file_path);

    if (file) {
        std::ostringstream buffer;

        // Extract all text from file
        buffer << file.rdbuf();

        result.status_line = "HTTP/1.1 200 OK";
        result.content_type = "text/html";
        result.body = buffer.str();
    } else {
        result.status_line = "HTTP/1.1 500 Internal Server Error";
        result.content_type = "text/plain";
        result.body = "500 - Aw man... the file couldn't be loaded :[";
    }

    return result;
}
