#pragma once

#include <winsock2.h>

class Socket {
    public:
        // Uninitialized socket handle
        Socket();

        // Assign socket parameter to handle
        explicit Socket(SOCKET s);

        ~Socket();

        // Prevent duplicate socket handles
        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        // Initialize by stealing handle from another socket
        Socket(Socket&& other) noexcept;

        // Transfer ownership from another socket
        Socket& operator=(Socket&& other) noexcept;

        SOCKET get() const;

        bool is_valid() const;

    private:
        SOCKET handle_;
};