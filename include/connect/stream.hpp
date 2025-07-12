/**
 * @file byteStream.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief ByteStream class definition.
 * @version 0.1
 * @date 2025-07-12
 * 
 * Copyright (c) Riley Horrix 2025
 */
#ifndef ABALONE_BYTESTREAM_H
#define ABALONE_BYTESTREAM_H

#include <cstddef>
#include <ostream>

namespace Abalone {

/**
 * @brief Generic interface for a stream of bytes.
 */
template<class T>
class Stream {
public:
    using input_stream = std::basic_istream<T>;
    using output_stream = std::basic_ostream<T>;

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
    virtual std::size_t write(T* buffer, std::size_t n) = 0;

    /**
     * @brief Get an input stream to read from.
     * 
     * @return std::basic_istream<T>& The input stream.
     */
    virtual input_stream& getRead(void) = 0;
    
    /**
     * @brief Get an output stream to write to.
     * 
     * @return std::basic_ostream<T>& The output stream.
     */
    virtual output_stream& getWrite(void) = 0;
};

}

#endif
