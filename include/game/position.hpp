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

    static Position invalid(void);

    /**
     * @brief Get the Horizontal position.
     * 
     * @return char The position character.
     */
    inline char getHorizontal(void) const { return horizontal; }

    /**
     * @brief Get the Diagonal position.
     * 
     * @return char The position character.
     */
    inline char getDiagonal(void) const { return diagonal; }

    /**
     * @brief Get whether the position is a valid board position or not.
     * 
     * @return true Valid position.
     * @return false Invalid position.
     */
    inline bool isValid(void) const { return valid; }

    /**
     * @brief Get the horizontal index of the position.
     * 
     * On this scale, 'a' is 0, and 'i' is 8.
     * 
     * @return int The horizontal index.
     */
    inline int getHorizontalIndex(void) const { return hInd; }

    /**
     * @brief Get the diagonal index of the position.
     * 
     * On this scale, '1' is 0, and '9' is 8.
     * 
     * @return int The diagonal index.
     */
    inline int getDiagonalIndex(void) const { return dInd; }

private:
    char horizontal;
    char diagonal;

    int hInd;
    int dInd;

    bool valid;
};

}