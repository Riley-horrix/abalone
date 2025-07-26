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

namespace Abalone {

class APIMessageStream : public Stream<std::unique_ptr<APIMessage>> {
public:
    /**
     * @brief Construct a new Message Stream object.
     * 
     * @param byteStream The bytestream to send the API messages.
     */
    APIMessageStream(std::unique_ptr<Stream<char>> byteStream);

    // read

private:
    /// @brief The stream to write messages to.
    std::unique_ptr<Stream<char>> stream;
};

}