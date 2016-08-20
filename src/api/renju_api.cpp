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
#include <utils/json.h>
#include <api/renju_api.h>

std::string RenjuAPI::generateMove(const char *boardString,
                                   int        aiPlayerID,
                                   int        serachDepth,
                                   int        numThreads) {
    // Check input data
    if (strlen(boardString) != 225 ||
        aiPlayerID  < 1 || aiPlayerID > 2 ||
        serachDepth < 1 ||
        numThreads  < 1) {
        return RenjuAPI::generateResultJson(nullptr, "Invalid input data.");
    }

    // Copy board
    char board[225];
    std::memcpy(board, boardString, 225);

    // Convert from string
    for (int i = 0; i < 225; i++) board[i] -= '0';

    std::unordered_map<std::string, std::string> data = {{"player_wins", "0"}, {"move_r", "1"}, {"move_c", "2"}};
    return RenjuAPI::generateResultJson(&data, "ok");
}

std::string RenjuAPI::renderBoard(const char *board) {
    std::string result = "";
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            result.push_back(RenjuAIUtils::getCell(board, r, c) + '0');
            result.push_back(' ');
        }
        result.push_back('\n');
    }
    return result;
}

std::string RenjuAPI::generateResultJson(const std::unordered_map<std::string, std::string> *data,
                                         const std::string &message) {
    nlohmann::json result;
    if (data != nullptr) {
        // Iterate through map
        for (auto pair : *data) {
            result["result"][pair.first] = pair.second;
        }
    } else {
        result["result"] = nullptr;
    }
    result["message"] = message;

    // Serialize
    return result.dump();
}
