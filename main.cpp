#include <iostream>
#include <optional>
#include <sstream>
#include <thread>       // Multithreading
#include <winsock2.h>   // Socket functions and types
#include <ws2tcpip.h>   // TCP/IP helpers

struct HttpRequestLine {
    std::string method;
    std::string path;
    std::string version;
};

std::optional<HttpRequestLine> parse_request_line(const std::string& request_data);
std::string build_http_response(const std::string& body);

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
        std::string request_data;

        while (true) {
            // Read data from connected socket
            bytes_received = recv( client_socket.get(), buffer, sizeof(buffer), 0 );

            if (bytes_received > 0) {
                // Stockpile bytes in a string
                request_data.append(buffer, bytes_received);

                // Check for end of HTTP request header
                if ( request_data.find("\r\n\r\n") != std::string::npos ) {
                    break;
                }

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

        // For testing
        std::cout << "====================\n";
        std::cout << "Full HTTP Request:\n" << request_data << std::endl;
        std::cout << "====================\n";

        // Parse HTTP request line
        std::optional<HttpRequestLine> parsed = parse_request_line(request_data);

        if (!parsed) {
            std::cerr << "Failed to parse HTTP request line :(" << std::endl;

            return;
        }

        std::cout << "Parsed Request Line:" << std::endl;
        std::cout << "Method: " << parsed->method << std::endl;
        std::cout << "Path: " << parsed->path << std::endl;
        std::cout << "Version: " << parsed->version << std::endl;

        // Respond to client
        int bytes_sent;
        std::string body = "Hello, client!";
        std::string response = build_http_response(body);

        // Note: Test using "curl.exe -i http://localhost:8080" to see raw response
        bytes_sent = send( client_socket.get(), response.c_str(), static_cast<int>( response.size() ), 0 );

        if (bytes_sent == SOCKET_ERROR) {
            std::cerr << "recv() failed!" << std::endl;
        }

        std::cout << "Client handler ending for socket " << client_socket.get() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in client handler: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception in client handler . . ." << std::endl;
    }
}

std::optional<HttpRequestLine> parse_request_line(const std::string& request_data) {
    // Read the request line
    std::istringstream request_stream(request_data);
    std::string request_line;
    
    if ( !std::getline(request_stream, request_line) ) {
        return std::nullopt;
    }

    // Strip trailing carriage return
    if ( !request_line.empty() && request_line.back() == '\r' ) {
        request_line.pop_back();
    }
    
    // Split request line into tokens
    std::istringstream line_stream(request_line);
    std::string method, path, version;

    if ( !(((line_stream >> method) >> path) >> version) ) {
        return std::nullopt;
    }

    return HttpRequestLine{ method, path, version };
}

std::string build_http_response(const std::string& body) {
    std::ostringstream response_stream;

    // Prepare status line, a couple headers, and the text body
    response_stream << "HTTP/1.1 200 OK\r\n"
                    << "Content-Type: text/plain\r\n"
                    << "Content-Length: " << body.length() << "\r\n"
                    << "\r\n"
                    << body;

    return response_stream.str();
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