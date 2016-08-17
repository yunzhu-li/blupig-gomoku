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

#ifndef INCLUDE_API_RENJU_API_H_
#define INCLUDE_API_RENJU_API_H_

#include <string>

class RenjuAPI {
public:
    RenjuAPI();
    ~RenjuAPI();
    static std::string renderBoard(const char *board);
    static std::string generateMove(char *boardBase64,
                                    int  aiPlayerID,
                                    int  serachDepth,
                                    int  numThreads);
};

#endif  // INCLUDE_API_RENJU_API_H_
