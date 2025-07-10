/**
 * @file controller.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of the board controller.
 * @version 0.1
 * @date 2025-07-01
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include "game/controller.hpp"

using namespace Abalone;

Controller::Controller(std::shared_ptr<Board> board): board(board) {}

bool Controller::makeMove(const Move& move, const Player& player) {
    if (gameOver != Player::NONE || currentTurn != player) {
        return false;
    }

    currentTurn = otherPlayer(player);

    bool moveValid = board->move(move, player);
    gameOver = board->gameOver();
    return moveValid;
}

Player Controller::getPieceAt(const Position& position) {
    return board->pieceAt(position);
}

Player Controller::getWinState(void) {
    return gameOver;
}

void Controller::reset(void) {
    currentTurn = Player::BLACK;
    board->reset();
}

void Controller::reset(GameOpening opening) {
    currentTurn = Player::BLACK;
    board->reset(opening);
}