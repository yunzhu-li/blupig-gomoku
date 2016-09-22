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

#include <ai/ai.h>
#include <ai/eval.h>
#include <ai/negamax.h>
#include <ai/utils.h>
#include <utils/globals.h>
#include <cstring>

void RenjuAI::generateMove(const char *gs,
                           int         player,
                           int         serach_depth,
                           int        *move_r,
                           int        *move_c,
                           int        *winning_player,
                           int        *eval_count,
                           int        *pm_count) {
    // Check arguments
    if (move_r == nullptr ||
        move_c == nullptr ||
        winning_player == nullptr) return;

    // Initialize counters
    g_eval_count = 0;
    g_pm_count = 0;

    // Initialize data
    *move_r = -1;
    *move_c = -1;
    *winning_player = 0;

    // Check if anyone wins the game
    *winning_player = RenjuAIEval::winningPlayer(gs);
    if (*winning_player != 0) return;

    // Copy game state
    char b[225];
    std::memcpy(b, gs, 225);

    // Run negamax
    RenjuAINegamax::heuristicNegamax(b, player, serach_depth, move_r, move_c);

    // Check if anyone wins the game
    // Execute the move
    std::memcpy(b, gs, 225);
    RenjuAIUtils::setCell(b, *move_r, *move_c, player);
    *winning_player = RenjuAIEval::winningPlayer(b);

    // Write counters
    if (eval_count != nullptr) *eval_count = g_eval_count;
    if (pm_count != nullptr) *pm_count = g_eval_count * 30;
}
