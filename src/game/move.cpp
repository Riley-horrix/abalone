/**
 * @file move.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of Move class.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include "game/move.hpp"

using namespace Abalone;

// --- Move ---

Move::Move(BroadsideMove move): var(move) {}

Move::Move(InlineMove move): var(move) {}

MoveType Move::type(void) const {
    return std::holds_alternative<InlineMove>(var) ? MoveType::INLINE : MoveType::BROADSIDE;
}

const std::variant<BroadsideMove, InlineMove>& Move::getMove(void) const {
    return var;
}

// --- BroadsideMove ---

BroadsideMove::BroadsideMove(const Position& first, const Position& last, const Position& firstEnd): first(first), last(last), firstEnd(firstEnd) {}

// --- InlineMove ---

InlineMove::InlineMove(const Position& start, const Position& end): start(start), end(end) {}