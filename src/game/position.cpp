/**
 * @file position.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Position class implementation.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <cctype>

#include "common/logging.hpp"
#include "game/position.hpp"

using namespace Abalone;

Position::Position(char h, char d): horizontal(static_cast<char>(std::tolower(h))), diagonal(d) {
    horizontalIndex = horizontal - 'a';
    diagonalIndex = diagonal - '1';

    // Validate position
    if (horizontal < 'a' || horizontal > 'i' || diagonal < '1' || diagonal > '9') {
        valid = false;
        return;
    }

    if (horizontal <= 'e' && diagonal > ('5' + horizontal - 'a')) {
        valid = false;
        return;
    }

    if (horizontal > 'e' && diagonal < '2' + horizontal - 'f') {
        valid = false;
        return;
    }

    valid = true;
}

Position Position::invalid(void) {
    return Position('0', '0');
}