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

int RenjuAINegamax::negamax(char *gs, int player, int depth, int *move_r, int *move_c) {
    // RenjuAIEval::test(gs);
    // return 0;

    // Initialize with a minimum score
    int max_score = INT_MIN;

    // Eval game state
    if (depth == 0) return RenjuAIEval::evalState(gs, player);

    // Loop through all cells
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            // Consider only empty cells
            if (RenjuAIUtils::getCell(gs, r, c) != 0) continue;

            // Skip remote cells (no pieces within 2 cells)
            if (RenjuAIUtils::remoteCell(gs, r, c)) continue;

            // Place piece
            RenjuAIUtils::setCell(gs, r, c, player);

            // Run negamax recursively
            int s = -negamax(gs,                   // Game state
                             player == 1 ? 2 : 1,  // Change player
                             depth - 1,            // Reduce depth by 1
                             nullptr,              // Result move not required
                             nullptr);

            // Remove piece
            RenjuAIUtils::setCell(gs, r, c, 0);

            // Update max score
            if (s > max_score) {
                max_score = s;
                if (move_r != nullptr) *move_r = r;
                if (move_c != nullptr) *move_c = c;
            }
        }
    }
    return max_score;
}
