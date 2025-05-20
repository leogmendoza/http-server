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

    WSACleanup();

    std::cout << "Passed!";

    return 0;
}