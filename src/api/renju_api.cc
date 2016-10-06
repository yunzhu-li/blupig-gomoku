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
#include <utils/globals.h>
#include <cstring>

bool RenjuAPI::generateMove(const char *gs_string, int ai_player_id,
                            int search_depth, int time_limit, int num_threads,
                            int *actual_depth, int *move_r, int *move_c, int *winning_player,
                            unsigned int *node_count, unsigned int *eval_count, unsigned int *pm_count) {
    // Check input data
    size_t gs_size = (size_t)g_board_size * g_board_size;
    if (strlen(gs_string) != gs_size ||
        ai_player_id  < 1 || ai_player_id > 2 ||
        search_depth == 0 || search_depth > 10 ||
        num_threads  < 1) {
        return false;
    }

    // Copy game state
    char *gs = new char[gs_size];
    std::memcpy(gs, gs_string, gs_size);

    // Convert from string
    gsFromString(gs_string, gs);

    // Generate move
    RenjuAI::generateMove(gs, ai_player_id, search_depth, time_limit, actual_depth,
                          move_r, move_c, winning_player, node_count, eval_count, pm_count);

    // Release memory
    delete[] gs;
    return true;
}

void RenjuAPI::gsFromString(const char *gs_string, char *gs) {
    size_t gs_size = (size_t)g_board_size * g_board_size;
    if (strlen(gs_string) != gs_size) return;

    for (int i = 0; i < gs_size; i++) {
        gs[i] = gs_string[i] - '0';
    }
}

std::string RenjuAPI::renderGameState(const char *gs) {
    std::string result = "";
    for (int r = 0; r < g_board_size; r++) {
        for (int c = 0; c < g_board_size; c++) {
            result.push_back(RenjuAIUtils::getCell(gs, r, c) + '0');
            result.push_back(' ');
        }
        result.push_back('\n');
    }
    return result;
}
