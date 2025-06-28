/**
 * @file textRenderer.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief A command line based text renderer.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <memory>

#include "game/board.hpp"
#include "graphics/renderer.hpp"

namespace Abalone {

class TextRenderer: public Renderer {
public:
    /**
     * @brief Construct a new Text Renderer object with an instance of
     * an Abalone board.
     * 
     * @param board The game board.
     */
    TextRenderer(std::shared_ptr<AbaloneBoard> board);

    //! @copydoc Renderer::draw().
    void draw(void) override;

private:
    //! @brief Pointer to the board instance this renderer is rendering.
    std::shared_ptr<AbaloneBoard> board = nullptr;

    //! @brief Character representing white pieces.
    const char white = 'W';

    //! @brief Character representing black pieces.
    const char black = 'B';

    //! @brief Character representing a blank space.
    const char none = '+';
};

}