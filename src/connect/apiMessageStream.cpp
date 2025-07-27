/**
 * @file apiMessage.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of the ApiMessageStream class.
 * @version 0.1
 * @date 2025-07-26
 * 
 * Copyright (c) Riley Horrix 2025
 */

#include "connect/stream.hpp"
#include "connect/apiMessageStream.hpp"

using namespace Abalone;

APIMessageStream::APIMessageStream(std::unique_ptr<Stream<char>> byteStream): stream(std::move(byteStream)) {}

APIMessageStream::APIMessageStream(APIMessageStream&& other) {
    stream = std::move(other.stream);
    memcpy(msgBuf.data(), other.msgBuf.data(), other.bufEnd);
    bufEnd = other.bufEnd;
}

APIMessageStream& APIMessageStream::operator=(APIMessageStream&& other) {
    stream = std::move(other.stream);
    memcpy(msgBuf.data(), other.msgBuf.data(), other.bufEnd);
    bufEnd = other.bufEnd;
    return *this;
}

std::size_t APIMessageStream::read(std::unique_ptr<APIMessage>* buffer, std::size_t n) {
    // Message index
    std::size_t msgInd = 0;
    // The head pointer into the buffer
    std::size_t bufInd = 0;
    // Pointer to the next message
    std::size_t msgEnd = 0;
    // Number of bytes read each iteration from the stream
    std::size_t bytesRead = 0;

    // Read in from the stream and append to any remaining data in the buffer
    while ((bytesRead = stream->read(msgBuf.data() + bufEnd, MAX_MSG_BUF_SIZE - bufEnd)) != 0) {
        // Now we want to read through the entire buffer
        bufEnd += bytesRead;
        while (bufInd < bufEnd) {
            // If we have already sent the required number of messages, move
            // the data to the start of the buffer and return
            if (msgInd == n) {
                // If the data is not at the start then return.
                if (bufInd != 0) {
                    memcpy(msgBuf.data(), msgBuf.data() + bufInd, bufEnd - bufInd);
                    bufEnd -= bufInd;
                }
                return msgInd;
            }

            // Find a delimiter in the msg buffer
            for (msgEnd = bufInd; msgEnd < bufEnd; msgEnd++) {
                if (msgBuf[msgEnd] == MESSAGE_DELIM) {
                    msgEnd++;
                    break;
                }
            }

            // If delimiter not found in buffer, then break
            if (msgEnd == bufEnd) {
                break;
            }

            // Extract the JSON from the message buffer
            std::string jsonStr = std::string(msgBuf.data() + bufInd, msgEnd - bufInd - 1);
            buffer[msgInd++] = APIMessage::fromString(jsonStr);

            bufInd = msgEnd;
        }
        
        // Now move buffer back to start and restart the loop
        if (bufInd != 0) {
            memcpy(msgBuf.data(), msgBuf.data() + bufInd, bufEnd - bufInd);
            bufEnd -= bufInd;
            bufInd = 0;
        }
    }

    return msgInd;
}

std::size_t APIMessageStream::write(const std::unique_ptr<APIMessage>* buffer, std::size_t n) {
    // Iterate over every message
    for (std::size_t i = 0; i < n; i++) {
        // Stringify msg as JSON
        std::string msg = buffer[i]->toString();
        std::size_t written = 0;

        // Write the entire message into the stream
        const char* msgStr = msg.c_str();
        while(written < msg.size()) {
            stream->write(msgStr + written, msg.size() - written);
        }

        // Write delimiter
        const char delim[] = { MESSAGE_DELIM };
        while (stream->write(delim, 1) == 0);
    }

    return n;
}

APIMessageStream& APIMessageStream::operator<<(const std::unique_ptr<APIMessage>& msg) {
    write(&msg, 1);
    return *this;
}

APIMessageStream& APIMessageStream::operator>>(std::unique_ptr<APIMessage>& msg) {
    read(&msg, 1);
    return *this;
}