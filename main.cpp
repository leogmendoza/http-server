#include <iostream>
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers
#include <thread>       // Multithreading

class Socket {
    public:
        // Uninitialized socket handle
        Socket(): handle_(INVALID_SOCKET) { }

        // Assign socket parameter to handle
        Socket(SOCKET s): handle_(s) { }

        ~Socket() {
            // Clean up valid sockets
            if (handle_ != INVALID_SOCKET) {
                closesocket(handle_);
            }
        }

        // Prevent duplicate socket handles
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        // Initialize by stealing handle from another socket
        Socket(Socket&& other) noexcept {
            handle_ = other.handle_;
            other.handle_ = INVALID_SOCKET;
        }

        // Transfer ownership from another socket
        Socket& operator=(Socket&& other) noexcept {
            if (this != &other) {
                if (handle_ != INVALID_SOCKET) {
                    closesocket(handle_);
                }
            }
            
            handle_ = other.handle_;
            other.handle_ = INVALID_SOCKET;

            return *this;
        }

        SOCKET get() const {
            return handle_;
        }

        bool is_valid() const {
            return (handle_ != INVALID_SOCKET);
        }

    private:
        SOCKET handle_;
};

class TcpServer {
    public:
        TcpServer() {
            // Initialize Winsock 2.2 (Windows Sockets API)
            if ( WSAStartup( MAKEWORD(2, 2), &wsa_data_ ) != 0 ) {
                throw std::runtime_error("WSAStartup failed!");
            }

            // Create listening socket
            server_socket_ = Socket(socket(
                AF_INET,        // IPv4 address family
                SOCK_STREAM,    // TCP socket type
                IPPROTO_TCP     // TCP protocol
            ));

            if ( !server_socket_.is_valid() ) {
                throw std::runtime_error("Listening socket creation failed!");
            }

            // Prepare address of listening socket
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);     // Ensure big-endianness
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

            // Bind listening socket to the prepared address
            if ( bind( server_socket_.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr) ) == SOCKET_ERROR ) {
                throw std::runtime_error("Bind failed!");
            }

            // Begin listening to incoming connections
            if ( listen( server_socket_.get(), SOMAXCONN ) == SOCKET_ERROR ) {
                throw std::runtime_error("Listen failed!");
            }
        }

        ~TcpServer() {
            // Clean up connections
            WSACleanup();
        }

        Socket accept_client() {
            // Create socket for a client connection
            Socket client_socket(accept(
                server_socket_.get(),      // Listening socket
                nullptr,            // Client address
                nullptr             // Length of client address
            ));

            // Handle client connection error
            if ( !client_socket.is_valid() ) {
                throw std::runtime_error("Accept failed!");
            }

            std::cout << "Client connected!" << std::endl;

            return client_socket;
        }

    private:
        WSADATA wsa_data_;
        Socket server_socket_;
};

void handle_client(Socket client_socket) {
    try {
        std::cout << "Client handler started for socket " << client_socket.get() << std::endl;

        char buffer[1024];
        int bytes_received;

        while (true) {
            // Read data from connected socket
            bytes_received = recv( client_socket.get(), buffer, sizeof(buffer) -1, 0 );

            if (bytes_received > 0) {
                // Null-terminate if successful
                buffer[bytes_received] = '\0';
                std::cout << "Received: " << buffer << std::endl;
            } else if (bytes_received == 0) {
                // Gracefully close connection
                std::cout << "Client disconnected!" << std::endl;
                break;
            } else {
                // Note: For some reason, curl does not gracefully disconnect :/
                std::cerr << "recv() failed!" << std::endl;
                break;
            }
        }

        std::cout << "Client handler ending for socket " << client_socket.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in client handler: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in client handler . . ." << std::endl;
    }
}

int main() {
    try {
        TcpServer server;

        while (true) {
            try {
                Socket client_socket = server.accept_client();

                // Create new thread to handle a socket
                std::thread client_thread( handle_client, std::move(client_socket) );

                // Run socket handler thread independently
                client_thread.detach();
            } catch (const std::exception &e) {
                std::cerr << "Client error: " << e.what() << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    std::cout << "Server shut down . . . Xp" << std::endl;

    return 0;
}