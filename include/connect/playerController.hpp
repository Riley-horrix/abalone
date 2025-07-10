/**
 * @file playerController.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Defines a class for managing a single connection to a player.
 * @version 0.1
 * @date 2025-07-01
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include "game/controller.hpp"

namespace Abalone {

class PlayerController {
public:
    /**
     * @brief Construct a new Player Controller object.
     * 
     * @param fifoPath Path to the fifo pipe.
     * @param controller The board controller for this game.
     */
    PlayerController(const std::string& fifoPath, std::shared_ptr<Controller> controller);
};

}