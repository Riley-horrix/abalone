/**
 * @file renderer.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Renderer class definition.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

namespace Abalone {

class Renderer {
public:
    /**
     * @brief Execute a draw call.
     */
    virtual void draw(void) = 0;
    
protected:
    Renderer();
};

}