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
#include <memory>

#include "common/utils.hpp"
#include "game/board.hpp"
#include "graphics/textRenderer.hpp"
#include "abalone.hpp"

using namespace Abalone;

int main(void) {
    std::shared_ptr<AbaloneBoard> board = std::make_shared<AbaloneBoard>(GameOpening::BELGIAN_DAISY);

    TextRenderer renderer(board);

    renderer.draw();

    bool mv = board->move(Move(InlineMove(Position('B', '1'), Position('C', '2'))), Player::BLACK);

    std::cout << mv << "\n";

    renderer.draw();

    mv = board->move(Move(InlineMove(Position('B', '6'), Position('C', '6'))), Player::WHITE);

    std::cout << mv << "\n";

    renderer.draw();

    return 0;
}