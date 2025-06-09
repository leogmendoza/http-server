#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include "socket.hpp"
#include "http_request.hpp"

class ClientHandler {
    public:
        ClientHandler(Socket socket, sockaddr_in client_addr);
        void run();

    private:
        Socket client_socket_;
        sockaddr_in client_addr_;
};
