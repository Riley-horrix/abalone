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

Player Abalone::otherPlayer(const Player& player) {
    if (player == Player::BLACK) {
        return Player::WHITE;
    } else if (player == Player::WHITE) {
        return Player::BLACK;
    }

    return Player::NONE;
}

constexpr int AbaloneBoard::STARTING_PIECES;

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

bool AbaloneBoard::moveValid(const Move& move, const Player& player) {
    if (move.type() == MoveType::BROADSIDE) {
        return broadsideMoveValid(std::get<BroadsideMove>(move.getMove()), player);
    } else {
        return inlineMoveValid(std::get<InlineMove>(move.getMove()), player);
    }
}

bool AbaloneBoard::move(const Move& move, const Player& player) {
    if (!moveValid(move, player)) {
        return false;
    }

    if (move.type() == MoveType::BROADSIDE) {
        broadsideMove(std::get<BroadsideMove>(move.getMove()), player);
    } else {
        inlineMove(std::get<InlineMove>(move.getMove()), player);
    }

    return true;
}

Player AbaloneBoard::pieceAt(char horizontal, char diagonal) {
    // Validate numbers
    return this->pieceAt(Position(horizontal, diagonal));
}

Player AbaloneBoard::pieceAt(const Position& position) {
    return pieceAt(positionToIndex(position));
}

int AbaloneBoard::positionToIndex(const Position& position) {
    if (!position.isValid()) {
        return -1;
    }

    int index = hLookUp[position.horizontalIndex] + dOffset[position.horizontalIndex] + position.diagonalIndex;

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

void AbaloneBoard::setPieceAt(const Position& position, const Player& player) {
    uint64_t mask = Utils::bit<uint64_t>(positionToIndex(position));

    if (player == Player::NONE) {
        whitePieces &= ~mask;
        blackPieces &= ~mask;
    } else if (player == Player::WHITE) {
        whitePieces |= mask;
        blackPieces &= ~mask;
    } else {
        blackPieces |= mask;
        whitePieces &= ~mask;
    }
}

int AbaloneBoard::distance(const Position& start, const Position& end) {
    return std::max(std::abs(start.diagonalIndex - end.diagonalIndex), std::abs(start.horizontalIndex - end.horizontalIndex));
}


Player Abalone::AbaloneBoard::gameOver(void) {
    if (STARTING_PIECES - Utils::bitCount(whitePieces) >= 6) {
        return Player::WHITE;
    } else if (STARTING_PIECES - Utils::bitCount(whitePieces) >= 6) {
        return Player::BLACK;
    }

    return Player::NONE;
}

bool AbaloneBoard::inlineMoveValid(const InlineMove& move, const Player& player) {
    // Validate moves and player
    if (!move.end.isValid() || !move.start.isValid() || player == Player::NONE) {
        return false;
    }

    // Ensure starting position is correct player
    if (pieceAt(move.start) != player) {
        return false;
    }

    // Ensure finishing position is 1 space from the starting move
    if (distance(move.start, move.end) > 1) {
        return false;
    }

    // Get the direction of the move
    int horizontalDir = move.end.horizontalIndex - move.start.horizontalIndex;
    int diagonalDir = move.end.diagonalIndex - move.start.diagonalIndex;

    // Get every piece from the start to the end of the board in the direction
    // of the move.
    Player spaces[9] = { Player::NONE };

    int i = 0;
    Position currentPos = move.start;

    while (currentPos.isValid() && i < 9) {
        spaces[i++] = pieceAt(currentPos);
        currentPos = Position(currentPos.horizontal + horizontalDir, currentPos.diagonal + diagonalDir);
    }

    // Now we want to analyse the spaces array.
    int playerPieces = 0;
    int otherPieces = 0;

    while (spaces[playerPieces++] == player) {}

    playerPieces--;

    // Should be 3 or less, and not at the end of the board
    if (playerPieces > 3 || playerPieces == i) {
        return false;
    }

    // Now there should be either 1 blank space, or < playerPieces 
    // enemy pieces.

    if (spaces[playerPieces] == Player::NONE) {
        return true;
    }

    // Count other player pieces
    while (spaces[playerPieces + otherPieces++] == otherPlayer(player) && playerPieces + otherPieces < i) {}

    otherPieces--;

    if (playerPieces <= otherPieces) {
        return false;
    }

    // Ensure there is either the end of the board or empty after
    if (i > playerPieces + otherPieces && spaces[playerPieces + otherPieces] != Player::NONE) {
        return false;
    }

    return true;
}

bool AbaloneBoard::broadsideMoveValid(const BroadsideMove& move, const Player& player) {
        // Validate moves and player
    if (!move.last.isValid() || !move.first.isValid() || !move.firstEnd.isValid() || player == Player::NONE) {
        return false;
    }

    // Ensure starting position is correct player
    if (pieceAt(move.first) != player || pieceAt(move.firstEnd) != player) {
        return false;
    }

    // Ensure broadside is only using 3 or less pieces
    int pieces = distance(move.first, move.last) + 1;
    if (pieces > 3) {
        return false;
    }

    // Ensure move is only 1 space away from start
    if (distance(move.first, move.firstEnd) > 1) {
        return false;
    }

    // Get the direction of the move
    int horizontalDir = move.firstEnd.horizontalIndex - move.first.horizontalIndex;
    int diagonalDir = move.firstEnd.diagonalIndex - move.first.diagonalIndex;

    // Ensure that the direction is not equal to the direction of the broadside.
    // Get the direction of the broadside
    int horizontalBroadside = move.last.horizontalIndex - move.first.horizontalIndex;
    int diagonalBroadside = move.last.diagonalIndex - move.first.diagonalIndex;

    // Ensure middle piece is also player
    if (pieces == 3 && pieceAt(Position(move.first.horizontal + horizontalBroadside, move.first.diagonal + diagonalBroadside)) != player) {
        return false;
    }

    if (std::abs(horizontalBroadside) == std::abs(horizontalDir) &&
        std::abs(diagonalBroadside) == std::abs(diagonalDir)) {
        return false;
    }

    // Now just ensure all of the moved to places are empty squares
    Position currentPosition = move.firstEnd;
    for (int i = 0; i < pieces; i++) {
        if (!currentPosition.isValid() || pieceAt(currentPosition) != Player::NONE) {
            return false;
        }
    }

    return true;
}

void AbaloneBoard::inlineMove(const InlineMove& move, const Player& player) {
    // Get the direction of the move
    int horizontalDir = move.end.horizontalIndex - move.start.horizontalIndex;
    int diagonalDir = move.end.diagonalIndex - move.start.diagonalIndex;

    // Now we just want to make the move
    Player next = Player::NONE;
    Position currentPos = move.start;

    while (currentPos.isValid()) {
        Player current = pieceAt(currentPos);

        setPieceAt(currentPos, next);

        next = current;
        currentPos = Position(currentPos.horizontal + horizontalDir, currentPos.diagonal + diagonalDir);

        if (next == Player::NONE) {
            break;
        }
    }
}

void AbaloneBoard::broadsideMove(const BroadsideMove& move, const Player& player) {
    // Get the direction of the move
    int horizontalDir = move.firstEnd.horizontalIndex - move.first.horizontalIndex;
    int diagonalDir = move.firstEnd.diagonalIndex - move.first.diagonalIndex;

    // Get the direction of the broadside
    int horizontalBroadside = move.last.horizontalIndex - move.first.horizontalIndex;
    int diagonalBroadside = move.last.diagonalIndex - move.first.diagonalIndex;

    Position currentPos = move.first;

    while (currentPos.diagonal != move.last.diagonal || currentPos.horizontal != move.last.horizontal) {
        setPieceAt(currentPos, Player::NONE);
        setPieceAt(Position(currentPos.horizontal + horizontalDir, currentPos.diagonal + diagonalDir), player);

        currentPos = Position(currentPos.horizontal + horizontalBroadside, currentPos.diagonal + diagonalBroadside);
    }
}