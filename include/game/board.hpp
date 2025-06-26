/**
 * @file board.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Board class.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once
#include <cstdint>

namespace Abalone {

enum class GameOpening {
    BELGIAN_DAISY,
    GERMAN_DAISY
};

/**
 * @brief Class for managing a single abalone board instance.
 * 
 * The board is represented with 2 64 bit numbers, representing the positions
 * of the white pieces, and positions of the black.
 * 
 * In standard terms, 
 * 
 *     I O O O O O
 *    H O O O O O O
 *   G + + O O O + +
 *  F + + + + + + + +
 * E + + + + + + + + +
 *  D + + + + + + + + 9
 *   C + + @ @ @ + + 8
 *    B @ @ @ @ @ @ 7
 *     A @ @ @ @ @ 6
 *        1 2 3 4 5
 * 
 * The 64 bit numbers will represent the grid squares from A1-A5 in the first
 * 5 bits, then B1-B6 in the next 6, etc.
 * 
 */
class AbaloneBoard {
public:
    /**
     * @brief Construct a new Abalone Board object with a standard opening.
     */
    AbaloneBoard(GameOpening opening);

    /**
     * @brief Construct a new Abalone Board object using a custom setup.
     * 
     * @param opening 
     */
    AbaloneBoard(uint64_t whiteOpening, uint64_t blackOpening);

    /**
     * @brief Destroy the Abalone Board object.
     */
    ~AbaloneBoard();

    /**
     * @brief Print the board state to console.
     */
    void show(void);

    /**
     * @brief Enum representing the individual player.
     */
    enum class Player {
        NONE = 0,
        BLACK = 1,
        WHITE = 2
    };

    /**
     * @brief Get the value of the position at the given horizontal row (A-I),
     * and given diagonal row (1-9).
     * 
     * @param horizontal Horizontal index.
     * @param diagonal Diagonal index.
     * @return Player The player at that position, or None.
     */
    Player operator()(char horizontal, char diagonal);

private:
    //! @brief Bit field containing the positions of the white pieces.
    uint64_t whitePieces = 0;

    //! @brief Number of black pieces off the board.
    int whiteOff = 0;

    //! @brief Bit field containing the positions of the black pieces.
    uint64_t blackPieces = 0;

    //! @brief Number of black pieces off the board.
    int blackOff = 0;
};

}