/**
 * @file board.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Board class.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <cctype>
#include <cmath>

#include "common/logging.hpp"
#include "game/board.hpp"

using namespace Abalone;

int AbaloneBoard::hLookUp[9] = {
    0, 5, 11, 18, 26, 35, 43, 50, 56
};

int AbaloneBoard::dOffset[9] = {
    0, 0, 0, 0, 0, -1, -2, -3, -4
};

AbaloneBoard::AbaloneBoard(GameOpening opening) {
    // Initialise pieces depending on the chosen game opening.
    switch (opening) {
        case GameOpening::BELGIAN_DAISY:
            whitePieces = 
                0b0000000001100001110000011000000000001100000111000011000000000;
            blackPieces = 
                0b0000011000011100000110000000000000000000110000011100001100000;
            break;
        case GameOpening::GERMAN_DAISY:
            whitePieces = 
                0b0001100011100001100000000000000000000000000011000011100011000;
            blackPieces =
                0b1100011100001100000000000000000000000000000000011000011100011;
            break;
        default:
            error("Tried to create an AbaloneBoard with an invalid opening");
    }
}

// AbaloneBoard::AbaloneBoard(uint64_t whiteOpening, uint64_t blackOpening) {
//     // Verify that both openings have 14 marbles.

// }

AbaloneBoard::~AbaloneBoard() {}

void AbaloneBoard::show() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < std::abs(4 - i); j++) {
            printf(" ");
        }

        printf("%c ", 'I' - i);

        for (int j = 0; j < (9 - std::abs(i - 4)); j++) {
            Player player = this->operator()('I' - i, '1' + (i > 3 ? 0 : 4 - i) + j);
            switch (player) {
                case Player::WHITE:
                    printf("@ ");
                    break;
                case Player::BLACK:
                    printf("0 ");
                    break;
                case Player::NONE:
                    printf("+ ");
                    break;
            }
        }

        if (i >= 5) {
            printf("%c", '9' - (i - 5));
        }

        printf("\n");
    }

    printf("       1 2 3 4 5\n\n");
    printf("White: @\n");
    printf("Black: 0\n\n");
}

Player AbaloneBoard::operator()(char horizontal, char diagonal) {
    // Validate numbers
    return this->operator()(Position(horizontal, diagonal));
}

Player AbaloneBoard::operator()(const Position& position) {
    return pieceAt(positionToIndex(position));
}

int AbaloneBoard::positionToIndex(const Position& position) {
    if (!position.isValid()) {
        return -1;
    }

    int index = hLookUp[position.getHorizontalIndex()] + dOffset[position.getHorizontalIndex()] + position.getDiagonalIndex();

    return index;
}

Position AbaloneBoard::indexToPosition(const int index) {
    if (index < 0 || index > 60) {
        return Position::invalid();
    }

    // Iterate through lookup
    int hIndex = 0;

    while (hLookUp[hIndex++] < index) {}

    hIndex--;

    return Position('a' + hIndex, '1' - dOffset[hIndex] + index - hLookUp[index]);
}

Player AbaloneBoard::pieceAt(int index) {
    return blackPieces & Utils::bit<uint64_t>(index) ? Player::BLACK : 
        (whitePieces & Utils::bit<uint64_t>(index) ? Player::WHITE : Player::NONE);
}
