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

#include <protocols/cli.h>
#include <api/renju_api.h>
#include <utils/json.h>
#include <utils/globals.h>
#include <ctime>
#include <cstring>

bool RenjuProtocolCLI::beginSession(int argc, char const *argv[]) {
    // Print usage if no arguments provided
    if (argc < 2) {
        std::cerr << "Usage: renju-parallel -s <state>        The game state (required)" << std::endl;
        std::cerr << "                      [-p <ai_player>]  AI player (1: black, 2: white; default: 1)" << std::endl;
        std::cerr << "                      [-d <depth>]      AI Search depth (default: 6)" << std::endl;
        std::cerr << "                      [-t <threads>]    Number of threads (default: 1)" << std::endl;
        return false;
    }

    // Initialize arguments
    g_board_size = 19;
    char gs_string[362] = {0};
    int ai_player  = 1;
    int num_threads  = 1;
    int search_depth = 1;

    // Iterate through arguments
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];

        if (strncmp(arg, "-s", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) continue;

            // Check length and copy
            if (strlen(argv[i + 1]) == 361)
                memcpy(gs_string, argv[i + 1], 362);

        } else if (strncmp(arg, "-p", 2) == 0) {
            // AI player ID
            if (i >= argc - 1) continue;
            ai_player = atoi(argv[i + 1]);

        } else if (strncmp(arg, "-d", 2) == 0) {
            // Search depth
            if (i >= argc - 1) continue;
            search_depth = atoi(argv[i + 1]);

        } else if (strncmp(arg, "-t", 2) == 0) {
            // Number of threads
            if (i >= argc - 1) continue;
            num_threads = atoi(argv[i + 1]);

        } else if (strncmp(arg, "test", 4) == 0) {
            // Build test data
            // for (int i = 0; i < 225; i++)
            //     gs_string[i] = '0';
            memcpy(gs_string, "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 362);
            ai_player = 1;
            search_depth = 8;
        }
    }

    std::string result = generateMove(gs_string, ai_player, search_depth, num_threads);
    std::cout << result << std::endl;

    return true;
}

std::string RenjuProtocolCLI::generateMove(const char *gs_string, int ai_player_id,
                                           int serach_depth, int num_threads) {
    // Record start time
    std::clock_t clock_begin = std::clock();

    // Generate move
    int move_r, move_c, winning_player, eval_count, pm_count;
    bool success = RenjuAPI::generateMove(gs_string, ai_player_id, serach_depth, num_threads,
                                          &move_r, &move_c, &winning_player, &eval_count, &pm_count);

    if (!success) return generateResultJson(nullptr, "Invalid input data.");

    // Calculate elapsed CPU time
    std::clock_t clock_end = std::clock();
    int cpu_time = (clock_end - clock_begin) * 1000 / CLOCKS_PER_SEC;

    // Generate result map
    std::unordered_map<std::string, std::string> data = {{"move_r", std::to_string(move_r)},
                                                         {"move_c", std::to_string(move_c)},
                                                         {"winning_player", std::to_string(winning_player)},
                                                         {"ai_player", std::to_string(ai_player_id)},
                                                         // {"serach_depth", std::to_string(serach_depth)},
                                                         {"cpu_time", std::to_string(cpu_time)},
                                                         {"num_threads", std::to_string(num_threads)},
                                                         {"eval_count", std::to_string(eval_count)},
                                                         {"pm_count", std::to_string(pm_count)},
                                                         {"cc_0", std::to_string(g_cc_0)},
                                                         {"cc_1", std::to_string(g_cc_1)}};

    // Result
    return generateResultJson(&data, "ok");
}

std::string RenjuProtocolCLI::generateResultJson(const std::unordered_map<std::string, std::string> *data,
                                                 const std::string &message) {
    nlohmann::json result;
    if (data != nullptr) {
        // Add all k-v pairs to the result map
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
