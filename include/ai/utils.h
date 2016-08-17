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

#include <string>

class RenjuAIUtils {
 public:
    RenjuAIUtils();
    ~RenjuAIUtils();

    static inline char getPiece(const char *board, int r, int c) {
        if (r < 0 || r >= 15 || c < 0 || c >= 15) return -1;
        return board[15 * r + c];
    }

    static inline bool setPiece(char *board, int r, int c, char value) {
        if (r < 0 || r >= 15 || c < 0 || c >= 15) return false;
        board[15 * r + c] = value;
        return true;
    }
};

#endif  // INCLUDE_AI_UTILS_H_
