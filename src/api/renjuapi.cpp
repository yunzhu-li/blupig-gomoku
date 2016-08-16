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
#include <utils/base64.h>
#include <api/renjuapi.h>

std::string RenjuAPI::generateMove(char *boardBase64,
                                   int  aiPlayerID,
                                   int  serachDepth,
                                   int  numThreads) {

    // Check arguments
    int dec_len = Base64decode_len(boardBase64);
    if (dec_len     < 16)                  return "";
    if (aiPlayerID  < 1 || aiPlayerID > 2) return "";
    if (serachDepth < 1)                   return "";
    if (numThreads  < 1)                   return "";

    // Decode board
    char *buf = new char[dec_len];
    Base64decode(buf, boardBase64);

    // Copy board
    char *board = new char[16];
    std::memcpy(board, buf, 16);

    // Free temporary buffer
    delete[] buf;

    return "{\"move\":[1, 2]}";
}

