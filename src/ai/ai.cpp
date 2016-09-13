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
#include <ai/negamax.h>
#include <cstring>

void RenjuAI::generateMove(const char *gs,
                           int         player,
                           int         serach_depth,
                           int        *move_r,
                           int        *move_c,
                           int        *winning_player) {
    // Check arguments
    if (move_r == nullptr ||
        move_c == nullptr ||
        winning_player == nullptr) return;

    // Initialize data
    *move_r = 0;
    *move_c = 0;
    *winning_player = 0;

    // Copy game state
    char b[225];
    std::memcpy(b, gs, 225);

    // Run negamax
    RenjuAINegamax::negamax(b, player, serach_depth, move_r, move_c);
}
