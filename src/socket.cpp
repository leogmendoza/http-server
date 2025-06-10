#include "socket.hpp"

// Uninitialized socket handle
Socket::Socket(): handle_(INVALID_SOCKET) { }

// Assign socket parameter to handle
Socket::Socket(SOCKET s): handle_(s) { }

Socket::~Socket() {
    // Clean up valid sockets
    if (handle_ != INVALID_SOCKET) {
        closesocket(handle_);
    }
}

// Initialize by stealing handle from another socket
Socket::Socket(Socket&& other) noexcept {
    handle_ = other.handle_;
    other.handle_ = INVALID_SOCKET;
}

// Transfer ownership from another socket
Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (handle_ != INVALID_SOCKET) {
            closesocket(handle_);
        }
    }
    handle_ = other.handle_;
    other.handle_ = INVALID_SOCKET;

    return *this;
}

SOCKET Socket::get() const {
    return handle_;
}

bool Socket::is_valid() const {
    return (handle_ != INVALID_SOCKET);
}