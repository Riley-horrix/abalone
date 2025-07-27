/**
 * @file socketStream.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of the SocketStream class.
 * @version 0.1
 * @date 2025-07-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include "connect/socketStream.hpp"

#include "common/logging.hpp"

using namespace Abalone;

SocketStream::SocketStream(const int fd): fd(fd) {
    // The fd should already be open so no need to do anything
}

SocketStream::SocketStream(SocketStream&& other): fd(other.fd) {
    other.fd = -1;
}

SocketStream& SocketStream::operator=(SocketStream&& other) {
    fd = other.fd;
    other.fd = -1;
    return *this;
}

SocketStream::~SocketStream() {
    cleanup();
}

std::size_t SocketStream::read(char* buffer, std::size_t n) {
    std::size_t received = 0;
    while (received < n) {
        ssize_t bytes = recv(fd, buffer + received, n - received, 0);

        if (bytes < 0) {
            stl_error(errno, "Failed to receive bytes from socket.");
            return received;
        }

        received += static_cast<std::size_t>(bytes);
    }

    return received;
}

std::size_t SocketStream::write(const char* buffer, std::size_t n) {
    std::size_t sent = 0;
    while (sent < n) {
        ssize_t bytes = send(fd, buffer + sent, n - sent, 0);

        if (bytes < 0) {
            stl_error(errno, "Failed to send bytes to socket");
            return sent;
        }

        sent += static_cast<std::size_t>(bytes);
    }

    return sent;
}

SocketStream& SocketStream::operator<<(const char& c) {
    write(&c, 1);
    return *this;
}

SocketStream& SocketStream::operator>>(char& c) {
    read(&c, 1);
    return *this;
}

void SocketStream::cleanup(void) {
    if (fd >= 0) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
        fd = -1;
    }
}