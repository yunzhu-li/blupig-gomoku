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

#ifndef INCLUDE_AI_UTILS_H_
#define INCLUDE_AI_UTILS_H_

#include <utils/globals.h>

class RenjuAIUtils {
 public:
    RenjuAIUtils();
    ~RenjuAIUtils();

    static inline char getCell(const char *gs, int r, int c) {
        if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return -1;
        return gs[g_board_size * r + c];
    }

    static inline bool setCell(char *gs, int r, int c, char value) {
        if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return false;
        gs[g_board_size * r + c] = value;
        return true;
    }

    static bool remoteCell(const char *gs, int r, int c);

    // Game state hashing
    static void zobristInit(int size, uint64_t *z1, uint64_t *z2);
    static uint64_t zobristHash(const char *gs, int size, uint64_t *z1, uint64_t *z2);
    static inline void zobristUpdate(uint64_t *state, uint64_t *z1, uint64_t *z2,
                                     int row_size, int r, int c, int player) {
        if (player == 1) {
            *state ^= z1[row_size * r + c];
        } else if (player == 2) {
            *state ^= z2[row_size * r + c];
        }
    }

};

#endif  // INCLUDE_AI_UTILS_H_
