#include <iostream>
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers

class TcpServer {
    public:
        TcpServer() {
            // Initialize Winsock 2.2 (Windows Sockets API)
            if ( WSAStartup( MAKEWORD(2, 2), &wsa_data ) != 0 ) {
                throw std::runtime_error("WSAStartup failed!");
            }

            // Create listening socket
            SOCKET server_socket = socket(
                AF_INET,        // IPv4 address family
                SOCK_STREAM,    // TCP socket type
                IPPROTO_TCP     // TCP protocol
            );

            if (server_socket == INVALID_SOCKET) {
                WSACleanup();
                throw std::runtime_error("Listening socket creation failed!");
            }

            // Prepare address of listening socket
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(8080);     // Ensure big-endianness
            server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

            // Bind listening socket to the prepared address
            if ( bind( server_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr) ) == SOCKET_ERROR ) {
                closesocket(server_socket);
                WSACleanup();
                throw std::runtime_error("Bind failed!");
            }

            // Begin listening to incoming connections
            if ( listen(server_socket, SOMAXCONN) == SOCKET_ERROR ) {
                std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
                closesocket(server_socket);
                WSACleanup();
                throw std::runtime_error("Listen failed!");
            }
        }

        ~TcpServer() {
            // Clean up connections

        }

    private:
        WSADATA wsa_data;
        SOCKET server_socket;
};

int main() {
    // Create socket for a client connection
    SOCKET client_socket = accept(
        server_socket,      // Listening socket
        nullptr,            // Client address
        nullptr             // Length of client address
    );

    // Handle client connection error
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
        closesocket(server_socket);
        WSACleanup();

        return 1;
    }

    // Clean up connections
    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    std::cout << "Server shut down . . . Xp";

    return 0;
}