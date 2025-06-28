/**
 * @file position.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Definition of an Abalone Position.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

namespace Abalone {

class Position {
public:
    /**
     * @brief Construct a new Position object using a horizontal and diagonal
     * position.
     * 
     * @param horizontal Character from 'A' to 'I'.
     * @param diagonal Character from '1' to '9'.
     */
    Position(char horizontal, char diagonal);

    /**
     * @brief Initialiser for a blank invalid position.
     * 
     * @return Position An invalid position.
     */
    static Position invalid(void);

    /**
     * @brief Get whether the position is a valid board position or not.
     * 
     * @return true Valid position.
     * @return false Invalid position.
     */
    inline bool isValid(void) const { return valid; }

    //! @brief Horizontal position.
    char horizontal;

    //! @brief Diagonal position.
    char diagonal;

    //! @brief Horizontal position index.
    int horizontalIndex;

    //! @brief Diagonal position index.
    int diagonalIndex;

private:
    //! @brief If the position is valid or not.
    bool valid;
};

}