/**
 * @file abalone.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief The abalone game main entry point.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <memory>

#include "common/utils.hpp"
#include "game/board.hpp"
#include "graphics/textRenderer.hpp"
#include "abalone.hpp"


int main(void) {
    std::shared_ptr<Abalone::AbaloneBoard> board = std::make_shared<Abalone::AbaloneBoard>(Abalone::GameOpening::BELGIAN_DAISY);

    Abalone::TextRenderer renderer(board);

    renderer.draw();

    return 0;
}