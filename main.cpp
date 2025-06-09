#include <iostream>
#include <optional>
#include <sstream>      // Request parsing
#include <fstream>      // File reading
#include <thread>       // Multithreading
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers
#include "tcp_server.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "router.hpp"
#include "logger.hpp"
#include "client_handler.hpp"

int main() {
    try {
        TcpServer server;

        while (true) {
            try {
                sockaddr_in client_addr{};
                Socket client_socket = server.accept_client(client_addr);

                // Create new thread to handle a socket
                std::thread client_thread( 
                    // Lambda function to execute in thread
                    []( Socket socket, sockaddr_in addr ) {
                        ClientHandler handler(std::move(socket), addr);
                        handler.run();
                    }, 
                    // Lambda args
                    std::move(client_socket), 
                    client_addr );

                // Run socket handler thread independently
                client_thread.detach();
            } catch (const std::exception &e) {
                Logger::error(std::string("Client error: ") + e.what());
            }
        }
    } catch (const std::exception &e) {
        Logger::error(std::string("Server error: ") + e.what());
    }
    Logger::warn("Server shutting down . . . Xp");

    return 0;
}