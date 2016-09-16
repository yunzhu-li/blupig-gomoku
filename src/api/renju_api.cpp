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

#include <api/renju_api.h>
#include <ai/ai.h>
#include <ai/utils.h>
#include <utils/json.h>
#include <cstring>
#include <ctime>

std::string RenjuAPI::generateMove(const char *gs_string,
                                   int ai_player_id,
                                   int serach_depth,
                                   int num_threads) {
    // Check input data
    if (strlen(gs_string) != 225 ||
        ai_player_id  < 1 || ai_player_id > 2 ||
        serach_depth < 1 || serach_depth > 10 ||
        num_threads  < 1) {
        return generateResultJson(nullptr, "Invalid input data.");
    }

    // Copy game state
    char gs[225];
    std::memcpy(gs, gs_string, 225);

    // Convert from string
    for (int i = 0; i < 225; i++) gs[i] -= '0';

    // Record start time
    std::clock_t clock_begin = std::clock();

    // Generate move
    int move_r, move_c, winning_player, eval_count;
    RenjuAI::generateMove(gs, ai_player_id, serach_depth, &move_r, &move_c, &winning_player, &eval_count);

    // Calculate elapsed CPU time
    std::clock_t clock_end = std::clock();
    int elapsed_time = (clock_end - clock_begin) * 1000 / CLOCKS_PER_SEC;

    // Generate result map
    std::unordered_map<std::string, std::string> data = {{"winning_player", std::to_string(winning_player)},
                                                         {"move_r", std::to_string(move_r)},
                                                         {"move_c", std::to_string(move_c)},
                                                         {"eval_count", std::to_string(eval_count)},
                                                         {"num_threads", std::to_string(serach_depth)},
                                                         {"serach_depth", std::to_string(serach_depth)},
                                                         {"elapsed_time", std::to_string(elapsed_time)}};
    return generateResultJson(&data, "ok");
}

std::string RenjuAPI::renderGameState(const char *gs) {
    std::string result = "";
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            result.push_back(RenjuAIUtils::getCell(gs, r, c) + '0');
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
