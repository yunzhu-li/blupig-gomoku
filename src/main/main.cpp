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

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <api/renjuapi.h>

std::string generateMoveWithCLIArgs(int argc, char const *argv[]) {
    // Initialize arguments
    char board_string[225] = {0};
    int ai_player  = 1;
    int num_threads  = 1;
    int search_depth = 1;

    // Iterate through arguments
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (strncmp(arg, "-b", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) continue;

            // Check length anc copy
            if (strlen(argv[i + 1]) == 225)
                memcpy(board_string, argv[i + 1], 225);

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
        }
    }

    std::string result = RenjuAPI::generateMove(board_string, ai_player, search_depth, num_threads);
    return result;
}

int main(int argc, char const *argv[]) {
    // Print usage if no arguments provided
    if (argc < 2) {
        printf("Usage: renju-parallel -b <board>        The 225-character board state (required)\n");
        printf("                      [-p <ai_player>]  AI player (1: black, 2: white; default: 1)\n");
        printf("                      [-d <depth>]      AI Search depth (default: xx)\n");
        printf("                      [-t <threads>]    Number of threads (default: 1)\n");
        return 1;
    }

    // Process arguments and generate move
    std::string result = generateMoveWithCLIArgs(argc, argv);
    printf("%s\n", result.c_str());

    return 0;
}
