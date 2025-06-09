#include <stdexcept>
#include "tcp_server.hpp"

TcpServer::TcpServer() {
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

TcpServer::~TcpServer() {
    // Clean up connections
    WSACleanup();
}

Socket TcpServer::accept_client(sockaddr_in& client_addr_output) {
    int addr_len = sizeof(client_addr_output);

    // Create socket for a client connection
    Socket client_socket(accept(
        server_socket_.get(),                               // Listening socket
        reinterpret_cast<sockaddr*>(&client_addr_output),   // Client address
        &addr_len                                           // Length of client address
    ));

    // Handle client connection error
    if ( !client_socket.is_valid() ) {
        throw std::runtime_error("Accept failed!");
    }
    
    return client_socket;
}