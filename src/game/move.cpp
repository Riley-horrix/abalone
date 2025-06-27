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

Move::Move(MoveType type): moveType(type) {}

MoveType Move::type(void) { return moveType; }

// --- BroadsideMove ---

BroadsideMove::BroadsideMove(const Position& first, const Position& last, const Position& firstEnd): Move(MoveType::BROADSIDE), first(first), last(last), firstEnd(firstEnd) {}

// --- InlineMove ---

InlineMove::InlineMove(const Position& start, const Position& end): Move(MoveType::INLINE), start(start), end(end) {}