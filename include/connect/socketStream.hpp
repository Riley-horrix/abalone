/**
 * @file socketStream.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief SocketStream class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include "connect/stream.hpp"

namespace Abalone {

/**
 * @brief Class to handle reading from and writing to a socket file descriptor.
 */
class SocketStream : public Stream<char> {
public:
    /**
     * @brief Construct a new Socket Stream object from its file descriptor.
     */
    explicit SocketStream(const int fd);

    /**
     * @brief Move constructor.
     */
    SocketStream(SocketStream&& other);

    /**
     * @brief Move assignment constructor.
     */
    SocketStream& operator=(SocketStream&& other);

    /// @brief Deleted copy constructor.
    SocketStream(const SocketStream&) = delete;
    
    /// @brief Deleted copy assignment constructor.
    SocketStream& operator=(const SocketStream&) = delete;

    /**
     * @brief Destroy the Socket Stream object.
     */
    ~SocketStream();

    /**
     * @brief Read n chars into the buffer.
     * 
     * @param buffer The buffer to read elements into.
     * @param n Number of elements to read.
     * @return std::size_t The number of elements read.
     */
    std::size_t read(char* buffer, std::size_t n) override;

    /**
     * @brief Write n chars of the buffer into the stream.
     * 
     * @param buffer The buffer to get elements from.
     * @param n Number of elements to write.
     * @return std::size_t Number of elements written.
     */
    std::size_t write(const char* buffer, std::size_t n) override;

    /**
     * @brief Overloaded write into operator.
     * 
     * @param c The character to write.
     * @return SocketStream& This.
     */
    SocketStream& operator<<(const char& c) override;

    /**
     * @brief Overloaded read from operator.
     * 
     * @param c The message to read into.
     * @return SocketStream& This.
     */
    SocketStream& operator>>(char& c) override;

private:
    /**
     * @brief Clean up and close the socket.
     */
    void cleanup(void);

    /// @brief The socket file descriptor.
    int fd = -1;
};

}