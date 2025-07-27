/**
 * @file messageStream.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief MessageStream class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include "connect/stream.hpp"
#include "connect/socketStream.hpp"
#include "connect/messages.hpp"

#define MAX_MSG_BUF_SIZE 1024

namespace Abalone {

class APIMessageStream : public Stream<std::unique_ptr<APIMessage>> {
public:
    /**
     * @brief Construct a new Message Stream object.
     * 
     * @param byteStream The bytestream to send the API messages.
     */
    APIMessageStream(std::unique_ptr<Stream<char>> byteStream);

    /**
     * @brief Move constructor.
     */
    APIMessageStream(APIMessageStream&& other);

    /**
     * @brief Move assignment constructor.
     */
    APIMessageStream& operator=(APIMessageStream&& other);

    /// @brief Deleted copy constructor.
    APIMessageStream(const APIMessageStream&) = delete;
    
    /// @brief Deleted copy assignment constructor.
    APIMessageStream& operator=(const APIMessageStream&) = delete;

        /**
     * @brief Read n elements of std::unique_ptr<APIMessage> into the buffer.
     * 
     * @param buffer The buffer to read elements into.
     * @param n Number of elements to read.
     * @return std::size_t The number of elements read.
     */
    std::size_t read(std::unique_ptr<APIMessage>* buffer, std::size_t n) override;

    /**
     * @brief Write n elements of the buffer into the stream.
     * 
     * @param buffer The buffer to get elements from.
     * @param n Number of elements to write.
     * @return std::size_t Number of elements written.
     */
    std::size_t write(const std::unique_ptr<APIMessage>* buffer, std::size_t n) override;

    /**
     * @brief Overloaded write into operator.
     * 
     * @param msg The message to write.
     * @return APIMessageStream& This.
     */
    APIMessageStream& operator<<(const std::unique_ptr<APIMessage>& msg) override;

    /**
     * @brief Overloaded read from operator.
     * 
     * @param msg The message to read into.
     * @return APIMessageStream& This.
     */
    APIMessageStream& operator>>(std::unique_ptr<APIMessage>& msg) override;

private:
    /// @brief The stream to write messages to.
    std::unique_ptr<Stream<char>> stream;

    /// @brief Raw message storage buffer.
    std::array<char, MAX_MSG_BUF_SIZE> msgBuf;

    /// @brief The number of characters in the buffer.
    std::size_t bufEnd = 0;

    /// @brief The character separating the JSON messages.
    // TODO: This will have to be changed as user messages are included.
    // It is also very vulnerable to attack.
    static constexpr char MESSAGE_DELIM = '\n';
};

}
