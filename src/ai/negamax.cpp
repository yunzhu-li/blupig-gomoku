/*
 * renju-parallel
 * Copyright (C) 2016 Yunzhu Li
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ai/negamax.h>
#include <ai/eval.h>
#include <ai/utils.h>
#include <climits>

int RenjuAINegamax::negamax(char *board,
                            int player,
                            int depth,
                            int *move_r,
                            int *move_c) {
    int max_score = INT_MIN;

    // Eval game
    if (depth == 0) return RenjuAIEval::evalState(board, player);

    // Loop through all cells
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            // Consider only empty cells
            if (RenjuAIUtils::getCell(board, r, c) != 0) continue;

            // Place piece
            RenjuAIUtils::setCell(board, r, c, player);

            // Change player
            player = player == 1 ? 2 : 1;

            // Run negamax recursively
            int s = -negamax(board, player, depth - 1, nullptr, nullptr);

            // Remove piece
            RenjuAIUtils::setCell(board, r, c, 0);

            if (s > max_score) {
                max_score = s;
                if (move_r != nullptr) *move_r = r;
                if (move_c != nullptr) *move_c = c;
            }
        }
    }
    return max_score;
}
