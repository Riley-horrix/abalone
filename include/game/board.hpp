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
#include <vector>

#include "game/move.hpp"
#include "game/position.hpp"

namespace Abalone {

enum class GameOpening {
    BELGIAN_DAISY,
    GERMAN_DAISY
};

/**
 * @brief Enum representing the individual player.
 */
enum class Player {
    NONE = 0,
    BLACK = 1,
    WHITE = 2
};

Player otherPlayer(const Player& player);

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

    // /**
    //  * @brief Construct a new Abalone Board object using a list of positions for 
    //  * the white and black pieces.
    //  */
    // AbaloneBoard(const std::vector<Position>& whitePositions, const std::vector<Position>& blackPositions);

    // /**
    //  * @brief Construct a new Abalone Board object using two bitfields.
    //  * 
    //  * @param whiteOpening bitField containing the white pieces.
    //  * @param blackOpening bitField containing the black pieces.
    //  */
    // AbaloneBoard(uint64_t whiteOpening, uint64_t blackOpening);

    /**
     * @brief Destroy the Abalone Board object.
     */
    ~AbaloneBoard();

    /**
     * @brief Validate a move for a given player.
     * 
     * @param move The move to make.
     * @param player The player making the move.
     * @return true The move is valid.
     * @return false The move is invalid.
     */
    bool moveValid(const Move& move, const Player& player);

    /**
     * @brief Make a move on the board. Returns true if the move is valid.
     * 
     * @param move The move to make;
     * @param player The player making the move.
     * @return bool Whether or not the move was valid.
     */
    bool move(const Move& move, const Player& player);

    /**
     * @brief Get the value of the position at the given horizontal row (A-I),
     * and given diagonal row (1-9).
     * 
     * @param horizontal Horizontal index.
     * @param diagonal Diagonal index.
     * @return Player The player at that position, or None.
     */
    Player pieceAt(char horizontal, char diagonal);

    /**
     * @brief Get the value of the position at the given position.
     * 
     * @param position The position.
     * @return Player The player at that position, or None.
     */
    Player pieceAt(const Position& position);

    /**
     * @brief Get the number of moves between the start and end positions.
     * 
     * @param start The starting position.
     * @param end The finishing position.
     * @return int The number of moves to move a piece from the start to the end, or -1 if either move is invalid.
     */
    int distance(const Position& start, const Position& end);

    /**
     * @brief Determines whether the game is over yet.
     * 
     * @return Player None if game is ongoing, or the winner.
     */
    Player gameOver(void);

private:
    //! @brief Bit field containing the positions of the white pieces.
    uint64_t whitePieces = 0;

    //! @brief Bit field containing the positions of the black pieces.
    uint64_t blackPieces = 0;

    static constexpr int STARTING_PIECES = 14;

    //! @brief Index offset lookup for horizontal positions.
    static int hLookUp[9];

    //! @brief Diagonal offset lookup for horizontal positions.
    static int dOffset[9];

    /**
     * @brief Translate a board position to an index as described in the board description.
     * 
     * @param position A board position.
     * @return int The index, or -1 if the position is invalid.
     */
    int positionToIndex(const Position& position);

    /**
     * @brief Translate an index to a board position.
     * 
     * @param index Index in the range [0, 60].
     * @return Position The position, this will be invalid if the index is 
     * out of range.
     */
    Position indexToPosition(const int index);

    /**
     * @brief Get the Player at the given index.
     * 
     * @param index The index into the bitfield.
     * @return Player The player, or None, at that position.
     */
    Player pieceAt(int index);

    void setPieceAt(const Position& position, const Player& player);

    bool inlineMoveValid(const InlineMove& move, const Player& player);
    bool broadsideMoveValid(const BroadsideMove& move, const Player& player);

    void inlineMove(const InlineMove& move, const Player& player);
    void broadsideMove(const BroadsideMove& move, const Player& player);
};

}