/**
 * @file controller.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Abalone game controller.
 * @version 0.1
 * @date 2025-06-29
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <memory>

#include "game/board.hpp"

namespace Abalone {

/**
 * @brief This class acts as the state controller for a board, implementing
 * functionality such as taking turns and win conditions.
 */
class Controller {
public:
    /**
     * @brief Construct a new Controller object using a board.
     * 
     * @param board The board to control.
     */
    Controller(std::shared_ptr<Board> board);

    /**
     * @brief Make a move on the board. Returns whether the move was made.
     * 
     * @param move The move to make.
     * @param player The player making the move.
     * @return bool Whether the move was valid or not.
     */
    bool makeMove(const Move& move, const Player& player);

    /**
     * @brief Get the value of the piece at a certain position.
     * 
     * @param position The position to get the piece of.
     * @return Player If the position is invalid, will return Player::NONE
     */
    Player getPieceAt(const Position& position);

    /**
     * @brief Get the winner of the current game.
     * 
     * @return Player The player that won, or NONE if the game is ongoing.
     */
    Player getWinState(void);

    /**
     * @brief Reset the game state.
     */
    void reset(void);

    /**
     * @brief Reset the game state with a specific opening.
     */
    void reset(GameOpening opening);

private:

    std::shared_ptr<Board> board = nullptr;

    Player currentTurn = Player::BLACK;

    Player gameOver = Player::NONE;
};

}