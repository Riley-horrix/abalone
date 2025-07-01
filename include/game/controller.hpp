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

class Controller {
    /**
     * @brief Construct a new Controller object using a board.
     * 
     * @param board The board to control.
     */
    Controller(std::shared_ptr<Board> board);

};

}