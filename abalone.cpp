/**
 * @file abalone.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief The abalone game main entry point.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <iostream>
#include <string>
#include <memory>

#include "common/utils.hpp"
#include "common/logging.hpp"
#include "game/board.hpp"
#include "graphics/textRenderer.hpp"

#include "abalone.hpp"

using namespace Abalone;

int main(void) {
    std::shared_ptr<Board> board = std::make_shared<Board>(GameOpening::BELGIAN_DAISY);
    std::unique_ptr<Renderer> renderer = std::make_unique<TextRenderer>(board);

    AbaloneGame game(board, std::move(renderer));

    game.start();

    return 0;
}

AbaloneGame::AbaloneGame(const std::shared_ptr<Board> board, std::unique_ptr<Renderer> renderer): board(board), renderer(std::move(renderer)) {}

void AbaloneGame::start(void) {
    Player winner;
    Player turn = Player::BLACK;
    while ((winner = board->gameOver()) == Player::NONE) {
        // Clear screen
        printf("\033[2J\033[1;1H\n\n\n");

        // Print Game State
        renderer->draw();

        // Get current turn
        do {
            prompt("Enter a valid move, %s: ", turn == Player::WHITE ? "white" : "black");
        } while (!board->move(readMove(), turn));

        turn = otherPlayer(turn);
    }
}

Move Abalone::AbaloneGame::readMove(void) {
    std::string line;
    std::cin >> line;

    if (line.length() == 4) {
        return Move(InlineMove(Position(line[0], line[1]), Position(line[2], line[3])));
    } else if (line.length() == 6) {
        return Move(BroadsideMove(Position(line[0], line[1]), Position(line[2], line[3]), Position(line[4], line[5])));
    } else {
        error("Invalid move length, must be 4 characters for inline or 6 for broadside.");
        return Move(InlineMove(Position::invalid(), Position::invalid()));
    }
}
