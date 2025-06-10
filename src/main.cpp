#include <iostream>
#include <thread>       // Multithreading
#include <mutex>        // Prevent race conditions

#include <winsock2.h>       // Socket functions and types
#include <ws2tcpip.h>       // TCP/IP helpers

#include "tcp_server.hpp"
#include "client_handler.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "router.hpp"
#include "logger.hpp"

// Client counter
int active_clients = 0;
std::mutex client_count_mutex;

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

                        // Increment client counter
                        {
                            std::lock_guard<std::mutex> lock(client_count_mutex);
                            /* Critical section begins */
                            ++active_clients;
                            /* Critical section ends */
                            
                            Logger::info( "Active clients: " + std::to_string(active_clients) );
                        }
                        handler.run();
                    }, 
                    // Lambda args
                    std::move(client_socket), 
                    client_addr 
                );

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