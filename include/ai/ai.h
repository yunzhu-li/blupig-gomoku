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

#ifndef INCLUDE_AI_AI_H_
#define INCLUDE_AI_AI_H_

class RenjuAI {
 public:
    RenjuAI();
    ~RenjuAI();

    static void generateMove(const char *gs, int player, int search_depth, int time_limit,
                             int *actual_depth, int *move_r, int *move_c, int *winning_player,
                             unsigned int *node_count, unsigned int *eval_count, unsigned int *pm_count);
};

#endif  // INCLUDE_AI_AI_H_
