/**
 * @file byteStream.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief ByteStream class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <cstddef>
#include <ostream>

namespace Abalone {

/**
 * @brief Generic interface for a stream of bytes.
 */
template<class T>
class Stream {
public:
    virtual ~Stream() {}
    
    /**
     * @brief Read n elements of T into the buffer.
     * 
     * @param buffer The buffer to read elements into.
     * @param n Number of elements to read.
     * @return std::size_t The number of elements read.
     */
    virtual std::size_t read(T* buffer, std::size_t n) = 0;

    /**
     * @brief Write n elements of the buffer into the stream.
     * 
     * @param buffer The buffer to get elements from.
     * @param n Number of elements to write.
     * @return std::size_t Number of elements written.
     */
    virtual std::size_t write(const T* buffer, std::size_t n) = 0;

    /**
     * @brief Overloaded write into operator.
     * 
     * @param msg The message to write.
     * @return Stream& This.
     */
    virtual Stream& operator<<(const T& msg) = 0;

    /**
     * @brief Overloaded read from operator.
     * 
     * @param msg The message to read into.
     * @return Stream& This.
     */
    virtual Stream& operator>>(T& msg) = 0;
};

}
