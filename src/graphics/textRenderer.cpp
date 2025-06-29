/**
 * @file textRenderer.cpp
 * @author rileyhorrix (riley@horrix.com)
 * @brief Implementation of text renderer class.
 * @version 0.1
 * @date 2025-06-27
 * 
 * Copyright (c) Riley Horrix 2025
 */
#include "common/logging.hpp"
#include "game/board.hpp"
#include "graphics/textRenderer.hpp"

using namespace Abalone;

TextRenderer::TextRenderer(std::shared_ptr<AbaloneBoard> board): board(board) {}

void TextRenderer::draw(void) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < std::abs(4 - i); j++) {
            printf(" ");
        }

        printf("%c ", 'I' - i);

        for (int j = 0; j < (9 - std::abs(i - 4)); j++) {
            Player player = board->pieceAt('I' - static_cast<char>(i), '1' + static_cast<char>((i > 3 ? 0 : 4 - i) + j));
            switch (player) {
                case Player::WHITE:
                    printf("%c ", white);
                    break;
                case Player::BLACK:
                    printf("%c ", black);
                    break;
                case Player::NONE:
                    printf("%c ", none);
                    break;
            }
        }

        if (i >= 5) {
            printf("%c", '9' - (i - 5));
        }

        printf("\n");
    }

    printf("       1 2 3 4 5\n\n");
    printf("White: %c\n", white);
    printf("Black: %c\n\n", black);
}