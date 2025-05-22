#include <iostream>
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers

int main() {
    // Initialize Winsock 2.2 (Windows Sockets API)
    WSADATA wsaData;
    int result = WSAStartup( MAKEWORD(2, 2), &wsaData );

    if ( result != 0 ) {
        std::cerr << "WSAStartup failed: " << result << std::endl;

        return 1;
    }

    // Create listening socket
    SOCKET server_socket = socket(
        AF_INET,        // IPv4 address family
        SOCK_STREAM,    // TCP socket type
        IPPROTO_TCP     // TCP protocol
    );

    // Prepare address of listening socket
    sockaddr_in server_addr{
        .sin_family = AF_INET,
        .sin_port = htons(8080),                            // Ensure big-endianness
        .sin_addr = { .s_addr = inet_addr("127.0.0.1") }
    };

    WSACleanup();

    std::cout << "Passed!";

    return 0;
}