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

Position::Position(char h, char d): horizontal(std::tolower(h)), diagonal(d) {
    hInd = horizontal - 'a';
    dInd = diagonal - '1';

    // Validate position
    if (horizontal < 'a' || horizontal > 'i' || diagonal < '1' || diagonal > '9') {
        warn("Bad board position! Index : (%c, %c), should be ([A,I], [1,9])", h, d);
        valid = false;
        return;
    }

    if (horizontal <= 'e' && diagonal > ('5' + horizontal - 'a')) {
        warn("Bad board position! Index : (%c, %c), should be ([A,I], [1,9])", h, d);
        valid = false;
        return;
    }

    if (horizontal > 'e' && diagonal < '2' + horizontal - 'f') {
        warn("Bad board position! Index : (%c, %c), should be ([A,I], [1,9])", h, d);
        valid = false;
        return;
    }

    valid = true;
}

Position Position::invalid(void) {
    return Position('0', '0');
}