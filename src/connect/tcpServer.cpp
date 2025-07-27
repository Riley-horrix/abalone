/**
 * @file tcpServer.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of the TCPServer class.
 * @version 0.1
 * @date 2025-07-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connect/tcpServer.hpp"

#include "common/logging.hpp"


using namespace Abalone;

TCPServer::TCPServer(const std::string& ipAddr, const uint16_t port, const int backlog): 
        ipAddr(ipAddr), port(port), backlog(backlog) {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (serverFd < 0) {
        stl_warn(errno, "Failed to start the TCP server socket");
        return;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ipAddr.c_str(), &addr.sin_addr) != 1) {
        stl_warn(errno, "Failed to translate ipaddr \"%s\"", ipAddr.c_str());
        serverFd = -1;
        return;
    }

    if (bind(serverFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        stl_warn(errno, "Failed to bind serverFd to the address");
        serverFd = -1;
        return;
    }

    if (listen(serverFd, backlog) == -1) {
        stl_warn(errno, "Failed to listen to socket file descriptor");
        serverFd = -1;
        return;
    }
}

TCPServer::TCPServer(TCPServer&& other): 
        ipAddr(other.ipAddr), port(other.port), backlog(other.backlog), 
        serverFd(other.serverFd), blockingAccept(other.blockingAccept), 
        openSockets(other.openSockets) {}

TCPServer::~TCPServer() {
    if (serverFd >= 0) {
        shutdown(serverFd, SHUT_RDWR);
        close(serverFd);
    }
}

std::shared_ptr<SocketStream> TCPServer::accept(bool block) {
    // If want to block and not currently blocking then set flags
    if (block && !blockingAccept) {
        int flags = fcntl(serverFd, F_GETFL, 0);
        if (flags == -1 || fcntl(serverFd, F_SETFL, flags | O_NONBLOCK) == -1) {
            stl_warn(errno, "Failed to get / set blocking flags on socket file descriptor");
            return nullptr;
        }
    }

    // If dont want to block and not currently blocking then clear flags
    if (!block && blockingAccept) {
        int flags = fcntl(serverFd, F_GETFL, 0);
        if (flags == -1 || fcntl(serverFd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
            stl_warn(errno, "Failed to get / set blocking flags on socket file descriptor");
            return nullptr;
        }
    }

    // Accept a connection
    int clientFd = -1;
    clientFd = ::accept(serverFd, nullptr, nullptr);

    // If the connection failed return nullptr
    if (clientFd < 0) {
        stl_warn(errno, "Failed to accept a connection to TCP server");
        return nullptr;
    }    

    // Return a new socketstream, this will handle the closing of the fd
    auto sock = std::make_shared<SocketStream>(clientFd);
    openSockets.push_back(sock);
    return sock;
}