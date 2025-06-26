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

AbaloneBoard::AbaloneBoard(uint64_t whiteOpening, uint64_t blackOpening) {
    // Verify that both openings have 14 marbles.

}

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

AbaloneBoard::Player AbaloneBoard::operator()(char horizontal, char diagonal) {
    // Validate numbers
    int hInd = std::tolower(horizontal) - 'a';
    int vInd = std::tolower(diagonal) - '1';

#ifdef DEBUG
    if (hInd < 0 || hInd > 9 || vInd < 0 || vInd > 9) {
        warn("Bad index into the board! Index : (%c, %c), should be ([A,I], [1,9])", horizontal, diagonal);
        return Player::NONE;
    }
#endif

    static int hLookUp[9] = {
        0, 5, 11, 18, 26, 35, 43, 50, 56
    };

    static int dOffset[9] = {
        0, 0, 0, 0, 0, -1, -2, -3, -4
    };

    int index = hLookUp[hInd] + dOffset[hInd] + vInd;

    return blackPieces & Utils::bit<uint64_t>(index) ? Player::BLACK : 
        (whitePieces & Utils::bit<uint64_t>(index) ? Player::WHITE : Player::NONE);
}