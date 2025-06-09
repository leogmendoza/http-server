#include <iostream>
#include <optional>
#include <sstream>      // Request parsing
#include <fstream>      // File reading
#include <thread>       // Multithreading
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers
#include "tcp_server.hpp"
#include "http_request.hpp"

// ANSI escape codes for coloured logging
const std::string RESET   = "\033[0m";
const std::string RED     = "\033[31m";
const std::string GREEN   = "\033[32m";
const std::string YELLOW  = "\033[33m";
const std::string CYAN    = "\033[36m";

std::string build_http_response(const std::string& status_line, const std::string& content_type, const std::string& body);

void handle_client(Socket client_socket, sockaddr_in client_addr) {
    try {
        // Log socket number and client IP address

        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

        std::cout << GREEN << "[INFO] Connection from " << ip_str 
                           << " on socket " << client_socket.get() 
                           << RESET << std::endl;

        // Receiving
        char buffer[1024];
        int bytes_received;
        std::string request_data;

        while (true) {
            // Read data from connected socket
            bytes_received = recv( client_socket.get(), buffer, sizeof(buffer), 0 );

            if (bytes_received > 0) {
                // Stockpile bytes in a string
                request_data.append(buffer, bytes_received);

                // Check for end of HTTP request header
                if ( request_data.find("\r\n\r\n") != std::string::npos ) {
                    break;
                }

            } else if (bytes_received == 0) {
                // Gracefully close connection
                std::cout << GREEN << "[INFO] Client disconnected!" << RESET << std::endl;

                break;
            } else {
                // Note: For some reason, curl does not gracefully disconnect :/
                std::cerr << RED << "[ERROR] recv() failed!" << RESET << std::endl;

                break;
            }
        }

        // Parse HTTP request line
        std::optional<HttpRequest> parsed = HttpRequest::parse(request_data);

        if (!parsed) {
            std::cerr << RED << "[ERROR] Failed to parse HTTP request line :(" << RESET << std::endl;

            return;
        }

        // Respond to client
        std::string body;
        std::string status_line = "HTTP/1.1 200 OK";
        std::string content_type = "text/plain";

        // Path handling -- TO DO: Move out / In general, split up handle_client
        if (parsed->path == "/") {
            // Note: Relative path accounts for building and executing within the `build` folder
            std::ifstream file("../public/index.html");

            if (file) {
                std::ostringstream string_stream;

                // Extract all text from file
                string_stream << file.rdbuf();

                content_type = "text/html";
                body = string_stream.str();
            } else {
                status_line = "HTTP/1.1 500 Internal Server Error";
                body = "Aw man, index.html could not be loaded :[";
            }
        } else if (parsed->path == "/about") {
            body =  "[TEST] This is Leo's HTTP server >B)";
        } else {
            status_line = "HTTP/1.1 404 Not Found";
            body = "[TEST] Ermmm.. unexpected path D:";
        }

        // Log parsed request information and response status
        std::cout << CYAN << "[INFO] " << parsed->method << " "
                          << parsed->path << " -> " << status_line.substr(9)
                          << RESET << std::endl;

        std::string response = build_http_response(status_line, content_type, body);

        // Sending
        size_t current_bytes = 0;
        size_t all_bytes = response.size();
        const char* data = response.c_str();

        // Avoid large file content from being cut off when sent
        while (current_bytes < all_bytes) {
            // Note: Test using "curl.exe -i http://localhost:8080" to see raw response
            int bytes_sent = send( client_socket.get(), data + current_bytes, static_cast<int>( all_bytes - current_bytes ), 0 );

            if (bytes_sent == SOCKET_ERROR) {
                std::cerr << RED << "[ERROR] send() failed!" << RESET << std::endl;
                break;
            }
            current_bytes += bytes_sent;
        }

        std::cout << GREEN << "[INFO] Client handler ending for socket " << client_socket.get() << RESET << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in client handler: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in client handler . . ." << std::endl;
    }
}

std::string build_http_response(const std::string& status_line, const std::string& content_type, const std::string& body) {
    std::ostringstream response_stream;

    // Prepare status line, a couple headers, and the text body
    response_stream << status_line << "\r\n"
                    << "Content-Type: " << content_type << "\r\n"
                    << "Content-Length: " << body.length() << "\r\n"
                    << "\r\n"
                    << body;

    return response_stream.str();
}

int main() {
    try {
        TcpServer server;

        while (true) {
            try {
                sockaddr_in client_addr{};
                Socket client_socket = server.accept_client(client_addr);

                // Create new thread to handle a socket
                std::thread client_thread( handle_client, std::move(client_socket), client_addr );

                // Run socket handler thread independently
                client_thread.detach();
            } catch (const std::exception &e) {
                std::cerr << "Client error: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    std::cout << YELLOW << "Server shut down . . . Xp" << RESET << std::endl;

    return 0;
}