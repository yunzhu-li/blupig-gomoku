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

#include <cstring>
#include <ai/utils.h>
#include <api/renjuapi.h>

std::string RenjuAPI::renderBoard(const char *board) {
    std::string result = "";
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            result.push_back(RenjuAIUtils::getPiece(board, r, c) + '0');
            result.push_back(' ');
        }
        result.push_back('\n');
    }
    return result;
}

std::string RenjuAPI::generateMove(char *boardString,
                                   int  aiPlayerID,
                                   int  serachDepth,
                                   int  numThreads) {
    // Check arguments
    if (strlen(boardString) != 225)        return "";
    if (aiPlayerID  < 1 || aiPlayerID > 2) return "";
    if (serachDepth < 1)                   return "";
    if (numThreads  < 1)                   return "";

    // Copy board
    char *board = new char[225];
    std::memcpy(board, boardString, 225);
    for (int i = 0; i < 225; i++) board[i] -= '0';

    std::string result = "{\"move\":[1, 2]}";

    // Free memory
    delete[] board;

    return result;
}

