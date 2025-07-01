/**
 * @file utils.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Utility function definitions.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <chrono>

#include "common/utils.hpp"

using namespace Abalone;

int Utils::bitCount(uint64_t number) {
    return bitCount(static_cast<uint32_t>(number & 0xffffffff)) + bitCount(static_cast<uint32_t>((number & 0xffffffff00000000) >> 16));
}

//! @copydoc Utils::bitCount(uint64_t)
int Utils::bitCount(uint32_t number) {
    return bitCount(static_cast<uint16_t>(number & 0xffff)) + bitCount(static_cast<uint16_t>((number & 0xffff0000) >> 16));
}

//! @copydoc Utils::bitCount(uint64_t)
int Utils::bitCount(uint16_t number) {
    return bitCount(static_cast<uint8_t>(number & 0xff)) + bitCount(static_cast<uint8_t>((number & 0xff00) >> 8));
}

//! @copydoc Utils::bitCount(uint64_t)
int Utils::bitCount(uint8_t number) {
    static uint8_t lookupTable[256] = {
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 
    };

    return static_cast<int>(lookupTable[number]);
}
uint64_t Utils::micros() {
    const std::chrono::system_clock::duration epoch = std::chrono::system_clock::now().time_since_epoch();
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(epoch).count());
}