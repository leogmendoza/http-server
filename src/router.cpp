#include <fstream>
#include <sstream>
#include "router.hpp"

RouteResult Router::route(const HttpRequest& request) {
    RouteResult result;

    result.status_line = "HTTP/1.1 200 OK";
    result.content_type = "text/plain";

    // Path handling -- TO DO: Move out / In general, split up handle_client
    if (request.path == "/") {
        // Note: Relative path accounts for building and executing within the `build` folder
        std::ifstream file("../public/index.html");

        if (file) {
            std::ostringstream string_stream;

            // Extract all text from file
            string_stream << file.rdbuf();

            result.content_type = "text/html";
            result.body = string_stream.str();
        } else {
            result.status_line = "HTTP/1.1 500 Internal Server Error";
            result.body = "Aw man, index.html could not be loaded :[";
        }
    } else if (request.path == "/about") {
        result.body =  "[TEST] This is Leo's HTTP server >B)";
    } else {
        result.status_line = "HTTP/1.1 404 Not Found";
        result.body = "[TEST] Ermmm.. unexpected path D:";
    }

    return result;
}
