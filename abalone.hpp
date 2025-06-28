/**
 * @file abalone.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief The abalone game object.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <memory>

#include "game/board.hpp"
#include "graphics/renderer.hpp"

namespace Abalone {

class AbaloneGame {
public:
    AbaloneGame(const std::shared_ptr<AbaloneBoard> board, std::unique_ptr<Renderer> renderer);

    void start(void);

    Move readMove(void);

private:
    const std::shared_ptr<AbaloneBoard> board;
    std::unique_ptr<Renderer> renderer;
};

}