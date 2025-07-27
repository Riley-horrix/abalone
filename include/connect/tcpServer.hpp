/**
 * @file tcpServer.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief TCP server class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <sys/socket.h>
#include <string>

#include "connect/socketStream.hpp"

namespace Abalone {

class TCPServer {
public:
    /**
     * @brief Construct a new TCPServer object at the given ip address and port number.
     * 
     * @param ipAddr The IP address (IPV4 / IPV6)
     * @param port The port number.
     * @param backlog Max backlock for listen connections.
     */
    explicit TCPServer(const std::string& ipAddr, const uint16_t port, int backlog = SOMAXCONN);

    /**
     * @brief Move constructor.
     */
    TCPServer(TCPServer&& other);

    /// @brief Deleted move assignment
    TCPServer& operator=(TCPServer&& other) = delete;

    /// @brief Deleted copy constructor.
    TCPServer(const TCPServer&) = delete;
    
    /// @brief Deleted copy assignment constructor.
    TCPServer& operator=(const TCPServer&) = delete;

    /**
     * @brief Destroy the TCPServer object.
     */
    ~TCPServer();

    /**
     * @brief Accept a new connection on the TCP server.
     * 
     * @param block Whether this is a blocking call or not.
     * @return SocketStream A socket stream to read / write to client. Returns nullptr if an error occurs.
     */
    std::shared_ptr<SocketStream> accept(bool block = true);

private:
    /// @brief The string ip address for the server.
    const std::string ipAddr;

    /// @brief The port number for the server.
    const uint16_t port;

    /// @brief The server connection backlog.
    const int backlog;

    /// @brief The server socket file descriptor.
    int serverFd;

    /// @brief Whether the accept call is blocking or not.
    bool blockingAccept = false;

    /// @brief The currently open sockets.
    std::vector<std::shared_ptr<SocketStream>> openSockets;
};

}