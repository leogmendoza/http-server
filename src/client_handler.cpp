#include "client_handler.hpp"

#include <string>
#include <optional>
#include <sstream>      // Request parsing

#include <winsock2.h>       // Socket functions and types
#include <ws2tcpip.h>       // TCP/IP helpers

#include "http_response.hpp"
#include "router.hpp"
#include "logger.hpp"

ClientHandler::ClientHandler(Socket socket, sockaddr_in client_addr): 
    client_socket_(std::move(socket)), client_addr_(client_addr) {}

void ClientHandler::run() {
    try {
        // Grab socket number and client IP address
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr_.sin_addr), ip_str, INET_ADDRSTRLEN);

        // Client connection
        Logger::info( std::string("Connection from ") 
                    + ip_str + " on socket " 
                    + std::to_string(client_socket_.get()) );

        // Receiving data
        char buffer[1024];
        int bytes_received;
        std::string request_data;

        while (true) {
            // Read data from connected socket
            bytes_received = recv( client_socket_.get(), buffer, sizeof(buffer), 0 );

            if (bytes_received > 0) {
                // Stockpile bytes in a string
                request_data.append(buffer, bytes_received);

                // Check for end of HTTP request header
                if ( request_data.find("\r\n\r\n") != std::string::npos ) {
                    break;
                }

            } else if (bytes_received == 0) {
                // Graceful client disconnection
                Logger::info("Client disconnected!");

                break;
            } else {
                // Note: For some reason, curl does not gracefully disconnect :/
                Logger::error("recv() failed!");

                break;
            }
        }

        // Parse HTTP request line
        std::optional<HttpRequest> parsed = HttpRequest::parse(request_data);

        if (!parsed) {
            Logger::error("Failed to parse HTTP request line :(");

            return;
        }

        // Respond to client
        RouteResult route = Router::route(*parsed);

        // Log parsed request information and response status
        std::ostringstream log_parsed;
        log_parsed << parsed->method << " " 
                   << parsed->path << " -> " 
                   << route.status_line.substr(9);

        Logger::status(log_parsed.str());

        std::string response;
        
        // Build response depending on file being served
        if (route.content_type.rfind("image/", 0) == 0) {
            response = HttpResponse::build_binary(route.status_line, route.content_type, route.body);
        } else {
            response = HttpResponse::build(route.status_line, route.content_type, route.body);
        }

        // Sending data
        size_t current_bytes = 0;
        size_t all_bytes = response.size();
        const char* data = response.c_str();

        // Avoid large file content from being cut off when sent
        while (current_bytes < all_bytes) {
            // Note: Test using "curl.exe -i http://localhost:8080" to see raw response
            int bytes_sent = send( client_socket_.get(), data + current_bytes, static_cast<int>( all_bytes - current_bytes ), 0 );

            if (bytes_sent == SOCKET_ERROR) {
                Logger::error("send() failed!");

                break;
            }
            current_bytes += bytes_sent;
        }

        Logger::info( "Client handler ending for socket " + std::to_string(client_socket_.get()) );
    } catch (const std::exception& e) {
        Logger::error( std::string("Exception in client handler: ") + e.what() );
    } catch (...) {
        Logger::error( "Unknown exception in client handler O_o" );
    }
}