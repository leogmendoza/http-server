#include <string>
#include <sstream>
#include <optional>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "client_handler.hpp"
#include "router.hpp"
#include "http_response.hpp"
#include "logger.hpp"

ClientHandler::ClientHandler(Socket socket, sockaddr_in client_addr): 
    client_socket_(std::move(socket)), client_addr_(client_addr) {}

void ClientHandler::run() {
    
}