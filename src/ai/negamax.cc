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

#include <ai/negamax.h>
#include <ai/eval.h>
#include <ai/utils.h>
#include <utils/globals.h>
#include <algorithm>
#include <climits>

// Breadth is used to control branching factor
// Different breadth configurations are possible:
// A lower breadth for a higher depth
// Or vice versa
#define kSearchBreadth 6

int RenjuAINegamax::heuristicNegamax(char *gs, int player, int depth,
                                     int *move_r, int *move_c) {
    // End condition
    if (depth == 0) return 0;

    int max_score = INT_MIN;
    int opponent = player == 1 ? 2 : 1;

    // Search and sort possible moves
    auto moves_player = searchMovesOrdered(gs, player);
    auto moves_opponent = searchMovesOrdered(gs, opponent);
    auto candidate_moves = new std::vector<RenjuAINegamax::Move>();

    // If opponent has good potential moves, consider blocking them first
    int tmp_size = std::min((int)moves_opponent->size(), 2);
    if (tmp_size > 0 && (*moves_opponent)[0].heuristic_val > 200) {
        for (int i = 0; i < tmp_size; ++i) {
            auto move = (*moves_opponent)[i];

            // Re-evaluate move as current player
            move.heuristic_val = RenjuAIEval::evalMove(gs, move.r, move.c, player);

            // Add to candidate list
            candidate_moves->push_back(move);
        }
    }

    // Copy moves for current player
    tmp_size = std::min((int)moves_player->size(), kSearchBreadth);
    for (int i = 0; i < tmp_size; ++i)
        candidate_moves->push_back((*moves_player)[i]);

    // Loop through every move
    int size = candidate_moves->size();
    for (int i = 0; i < size; i++) {
        auto move = (*candidate_moves)[i];

        // Stop if a winning move is found
        if (move.heuristic_val >= 50000) {
            max_score = move.heuristic_val;
            if (move_r != nullptr) *move_r = move.r;
            if (move_c != nullptr) *move_c = move.c;
            break;
        }

        // Execute move
        RenjuAIUtils::setCell(gs, move.r, move.c, player);

        // Run negamax recursively
        int score = heuristicNegamax(gs,         // Game state
                                     opponent,   // Change player
                                     depth - 1,  // Reduce depth by 1
                                     nullptr,    // Result move not required
                                     nullptr);

        // A little bit more aggressive
        if (score > 10) score *= 0.9;

        // Restore
        RenjuAIUtils::setCell(gs, move.r, move.c, 0);

        // Update maximum score
        if (move.heuristic_val - score > max_score) {
            max_score = move.heuristic_val - score;
            if (move_r != nullptr) *move_r = move.r;
            if (move_c != nullptr) *move_c = move.c;
        }

        // An experimental pruning
        // if (score < 100 && i >= 15) break;
    }

    // Release memory
    delete moves_player;
    delete moves_opponent;
    delete candidate_moves;

    return max_score;
}

std::vector<RenjuAINegamax::Move> *RenjuAINegamax::searchMovesOrdered(const char *gs, int player) {
    std::vector<Move> *result = new std::vector<Move>();

    // Loop through all cells
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            // Consider only empty cells
            if (RenjuAIUtils::getCell(gs, r, c) != 0) continue;

            // Skip remote cells (no pieces within 2 cells)
            if (RenjuAIUtils::remoteCell(gs, r, c)) continue;

            Move m;
            m.r = r;
            m.c = c;

            // Evaluate move
            m.heuristic_val = RenjuAIEval::evalMove(gs, r, c, player);

            // Add move
            result->push_back(m);
        }
    }
    std::sort(result->begin(), result->end());
    return result;
}

int RenjuAINegamax::negamax(char *gs, int player, int depth, int *move_r, int *move_c) {
    // Initialize with a minimum score
    int max_score = INT_MIN;

    // Eval game state
    if (depth == 0) return RenjuAIEval::evalState(gs, player);

    // Loop through all cells
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            // Consider only empty cells
            if (RenjuAIUtils::getCell(gs, r, c) != 0) continue;

            // Skip remote cells (no pieces within 2 cells)
            if (RenjuAIUtils::remoteCell(gs, r, c)) continue;

            // Execute move
            RenjuAIUtils::setCell(gs, r, c, player);

            // Run negamax recursively
            int s = -negamax(gs,                   // Game state
                             player == 1 ? 2 : 1,  // Change player
                             depth - 1,            // Reduce depth by 1
                             nullptr,              // Result move not required
                             nullptr);

            // Restore
            RenjuAIUtils::setCell(gs, r, c, 0);

            // Update max score
            if (s > max_score) {
                max_score = s;
                if (move_r != nullptr) *move_r = r;
                if (move_c != nullptr) *move_c = c;
            }
        }
    }
    return max_score;
}
