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
};

#endif  // INCLUDE_AI_UTILS_H_
