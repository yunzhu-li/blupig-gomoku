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

bool processArguments(int        argc,
                      char const *argv[],
                      char       *board_b64,
                      int        &aiPlayerID,
                      int        &serachDepth,
                      int        &numThreads) {

    // Initialize arguments
    memset(board_b64, 0, 25);
    aiPlayerID  = -1;
    numThreads  = -1;
    serachDepth = -1;

    // Iterate through arguments
    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (strncmp(arg, "-b", 2) == 0) {
            // Check if value exists
            if (i >= argc - 1) return false;

            // Check value length
            if (strlen(argv[i + 1]) != 24) return false;

            // Copy board
            strncpy(board_b64, argv[i + 1], 24);

        } else if (strncmp(arg, "-p", 2) == 0) {
            // AI player ID
            if (i >= argc - 1) return false;
            aiPlayerID = atoi(argv[i + 1]);

        } else if (strncmp(arg, "-t", 2) == 0) {
            // Number of threads
            if (i >= argc - 1) return false;
            numThreads = atoi(argv[i + 1]);

        } else if (strncmp(arg, "-d", 2) == 0) {
            // Search depth
            if (i >= argc - 1) return false;
            serachDepth = atoi(argv[i + 1]);
        }
    }

    return true;
}

int main(int argc, char const *argv[]) {

    // Process arguments
    char board_b64[25];
    int ai_player, num_threads, search_depth;
    if (!processArguments(argc, argv, board_b64, ai_player, search_depth, num_threads)) {
        printf("{\"result\": false, \"message\": \"Invalid arguments\"}\n");
        return 1;
    }

    // Call RenjuAPI and print results
    std::string result = RenjuAPI::generateMove(board_b64, ai_player, search_depth, num_threads);
    printf("%s\n", result.c_str());

    return 0;
}
