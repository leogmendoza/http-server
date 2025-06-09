#pragma once

#include <winsock2.h>
#include "socket.hpp"

class TcpServer {
    public:
        TcpServer();

        ~TcpServer();

        Socket accept_client(sockaddr_in& client_addr_output);

    private:
        WSADATA wsa_data_;
        Socket server_socket_;
};