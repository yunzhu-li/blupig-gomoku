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
#include <cstring>
#include <iostream>

std::string generateMoveWithCLIArgs(int argc, char const *argv[]) {
    // Initialize arguments
    char gs_string[226] = {0};
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
            if (strlen(argv[i + 1]) == 225)
                memcpy(gs_string, argv[i + 1], 226);

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
            memcpy(gs_string, "000000000000000000000000000000000000000000000000000001000000000000002000000000000001010000000000021000000000000211000000000000222000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 226);
            search_depth = 4;
        }
    }

    std::string result = RenjuAPI::generateMove(gs_string, ai_player, search_depth, num_threads);
    return result;
}

// Exclude main() if building with tests
#ifndef RENJU_PARALLEL_TEST

int main(int argc, char const *argv[]) {
    // Print usage if no arguments provided
    if (argc < 2) {
        std::cerr << "Usage: renju-parallel -s <state>        The 225-character game state (required)" << std::endl;
        std::cerr << "                      [-p <ai_player>]  AI player (1: black, 2: white; default: 1)" << std::endl;
        std::cerr << "                      [-d <depth>]      AI Search depth (default: xx)" << std::endl;
        std::cerr << "                      [-t <threads>]    Number of threads (default: 1)" << std::endl;
        return 1;
    }

    // Process arguments and generate move
    std::string result = generateMoveWithCLIArgs(argc, argv);
    printf("%s\n", result.c_str());

    return 0;
}

#endif  // RENJU_PARALLEL_TEST
