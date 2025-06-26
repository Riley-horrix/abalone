/**
 * @file abalone.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief The abalone game main entry point.
 * @version 0.1
 * @date 2025-06-26
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include <iostream>

#include "common/utils.hpp"
#include "game/board.hpp"
#include "abalone.hpp"


int main(void) {
    std::cout << "Hello, abalone " << std::endl;
    Abalone::Abalone();

    Abalone::AbaloneBoard board(Abalone::GameOpening::BELGIAN_DAISY);

    board.show();

    return 0;
}