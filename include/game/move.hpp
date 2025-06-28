/**
 * @file move.hpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Definition of the Move class.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#pragma once

#include <variant>

#include "game/position.hpp"

namespace Abalone {

enum class MoveType {
    INLINE,
    BROADSIDE
};

class BroadsideMove {
public:
    /**
     * @brief Construct a new Broadside Move.
     * 
     * @param first The position of the first ball in the broadside to move.
     * @param last The position of the last ball in the broadside to move.
     * @param firstEnd The position of the first ball after the move.
     */
    BroadsideMove(const Position& first, const Position& last, const Position& firstEnd);

    //! @brief The position of the first ball in the broadside to move.
    const Position first;

    //! @brief The position of the last ball in the broadside to move.
    const Position last;

    //! @brief The position of the first ball after the move.
    const Position firstEnd;
};

class InlineMove {
public:
    /**
     * @brief Construct a new Inline Move.
     * 
     * @param start The starting position of the last ball of the inline move.
     * @param end The finishing position of the last ball of the inline move.
     */
    InlineMove(const Position& start, const Position& end);

    //! @brief The starting position of the last ball of the inline move.
    const Position start;

    //! @brief The finishing position of the last ball of the inline move.
    const Position end;
};

class Move {
public:
    Move(BroadsideMove move);
    Move(InlineMove move);

    /**
     * @brief Get the type of move.
     * 
     * @return MoveType The type of move.
     */
    MoveType type(void) const;

    const std::variant<BroadsideMove, InlineMove>& getMove(void) const;

private:
    std::variant<BroadsideMove, InlineMove> var;
};

}