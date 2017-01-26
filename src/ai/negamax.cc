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
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

// kSearchBreadth is used to control branching factor
// Different breadth configurations are possible:
// A lower breadth for a higher depth
// Or vice versa
int RenjuAINegamax::presetSearchBreadth[5] = {10, 6, 6, 3, 3};

// Estimated average branching factor for iterative deepening
#define kAvgBranchingFactor 3

// Maximum depth for iterative deepening
#define kMaximumDepth 16

// kScoreDecayFactor decays score each layer so the algorithm
// prefers closer advantages
#define kScoreDecayFactor 0.95f

void RenjuAINegamax::heuristicNegamax(const char *gs, int player, int depth, int time_limit, bool enable_ab_pruning,
                                      int *actual_depth, int *move_r, int *move_c) {
    // Check arguments
    if (gs == nullptr ||
        player < 1 || player > 2 ||
        depth == 0 || depth < -1 ||
        time_limit < 0) return;

    // Copy game state
    char *_gs = new char[g_gs_size];
    memcpy(_gs, gs, g_gs_size);

    // Speedup first move
    int _cnt = 0;
    for (int i = 0; i < static_cast<int>(g_gs_size); i++)
        if (_gs[i] != 0) _cnt++;

    if (_cnt <= 2) depth = 6;

    // Fixed depth or iterative deepening
    if (depth > 0) {
        if (actual_depth != nullptr) *actual_depth = depth;
        heuristicNegamax(_gs, player, depth, depth, enable_ab_pruning,
                         INT_MIN / 2, INT_MAX / 2, move_r, move_c);
    } else {
        // Iterative deepening
        std::clock_t c_start = std::clock();
        for (int d = 6;; d += 2) {
            std::clock_t c_iteration_start = std::clock();

            // Reset game state
            memcpy(_gs, gs, g_gs_size);

            // Execute negamax
            heuristicNegamax(_gs, player, d, d, enable_ab_pruning,
                             INT_MIN / 2, INT_MAX / 2, move_r, move_c);

            // Times
            std::clock_t c_iteration = (std::clock() - c_iteration_start) * 1000 / CLOCKS_PER_SEC;
            std::clock_t c_elapsed = (std::clock() - c_start) * 1000 / CLOCKS_PER_SEC;

            if (c_elapsed + (c_iteration * kAvgBranchingFactor * kAvgBranchingFactor) > time_limit ||
                d >= kMaximumDepth) {
                if (actual_depth != nullptr) *actual_depth = d;
                break;
            }
        }
    }
    delete[] _gs;
}

int RenjuAINegamax::heuristicNegamax(char *gs, int player, int initial_depth, int depth,
                                     bool enable_ab_pruning, int alpha, int beta,
                                     int *move_r, int *move_c) {
    // Count node
    ++g_node_count;

    int max_score = INT_MIN;
    int opponent = player == 1 ? 2 : 1;

    // Search and sort possible moves
    std::vector<Move> moves_player, moves_opponent, candidate_moves;
    searchMovesOrdered(gs, player, &moves_player);
    searchMovesOrdered(gs, opponent, &moves_opponent);

    // End if no move could be performed
    if (moves_player.size() == 0) return 0;

    // End directly if only one move or a winning move is found
    if (moves_player.size() == 1 || moves_player[0].heuristic_val >= kRenjuAiEvalWinningScore) {
        auto move = moves_player[0];
        if (move_r != nullptr) *move_r = move.r;
        if (move_c != nullptr) *move_c = move.c;
        return move.heuristic_val;
    }

    // If opponent has threatening moves, consider blocking them first
    bool block_opponent = false;
    int tmp_size = std::min(static_cast<int>(moves_opponent.size()), 2);
    if (moves_opponent[0].heuristic_val >= kRenjuAiEvalThreateningScore) {
        block_opponent = true;
        for (int i = 0; i < tmp_size; ++i) {
            auto move = moves_opponent[i];

            // Re-evaluate move as current player
            move.heuristic_val = RenjuAIEval::evalMove(gs, move.r, move.c, player);

            // Add to candidate list
            candidate_moves.push_back(move);
        }
    }

    // Set breadth
    int breadth = (initial_depth >> 1) - ((depth + 1) >> 1);
    if (breadth > 4) breadth = presetSearchBreadth[4];
    else             breadth = presetSearchBreadth[breadth];

    // Copy moves for current player
    tmp_size = std::min(static_cast<int>(moves_player.size()), breadth);
    for (int i = 0; i < tmp_size; ++i)
        candidate_moves.push_back(moves_player[i]);

      // Print heuristic values for debugging
//    if (depth >= 8) {
//        for (int i = 0; i < moves_player.size(); ++i) {
//            auto move = moves_player[i];
//            std::cout << depth << " | " << move.r << ", " << move.c << ": " << move.heuristic_val << std::endl;
//        }
//    }

    // Loop through every move
    int size = static_cast<int>(candidate_moves.size());
    for (int i = 0; i < size; ++i) {
        auto move = candidate_moves[i];

        // Execute move
        RenjuAIUtils::setCell(gs, move.r, move.c, static_cast<char>(player));

        // Run negamax recursively
        int score = 0;
        if (depth > 1) score = heuristicNegamax(gs,                 // Game state
                                                opponent,           // Change player
                                                initial_depth,      // Initial depth
                                                depth - 1,          // Reduce depth by 1
                                                enable_ab_pruning,  // Alpha-Beta
                                                -beta,              //
                                                -alpha + move.heuristic_val,
                                                nullptr,            // Result move not required
                                                nullptr);

        // Closer moves get more score
        if (score >= 2) score = static_cast<int>(score * kScoreDecayFactor);

        // Calculate score difference
        move.actual_score = move.heuristic_val - score;

        // Store back to candidate array
        candidate_moves[i].actual_score = move.actual_score;

        // Print actual scores for debugging
//        if (depth >= 8)
//            std::cout << depth << " | " << move.r << ", " << move.c << ": " << move.actual_score << std::endl;

        // Restore
        RenjuAIUtils::setCell(gs, move.r, move.c, 0);

        // Update maximum score
        if (move.actual_score > max_score) {
            max_score = move.actual_score;
            if (move_r != nullptr) *move_r = move.r;
            if (move_c != nullptr) *move_c = move.c;
        }

        // Alpha-beta
        int max_score_decayed = max_score;
        if (max_score >= 2) max_score_decayed = static_cast<int>(max_score_decayed * kScoreDecayFactor);
        if (max_score > alpha) alpha = max_score;
        if (enable_ab_pruning && max_score_decayed >= beta) break;
    }

    // If no moves that are much better than blocking threatening moves, block them.
    // This attempts blocking even winning is impossible if the opponent plays optimally.
    if (depth == initial_depth && block_opponent && max_score < 0) {
        auto blocking_move = candidate_moves[0];
        int b_score = blocking_move.actual_score;
        if (b_score == 0) b_score = 1;
        if ((max_score - b_score) / static_cast<float>(std::abs(b_score)) < 0.2) {
            if (move_r != nullptr) *move_r = blocking_move.r;
            if (move_c != nullptr) *move_c = blocking_move.c;
            max_score = blocking_move.actual_score;
        }
    }
    return max_score;
}

void RenjuAINegamax::searchMovesOrdered(const char *gs, int player, std::vector<Move> *result) {
    // Clear and previous result
    result->clear();

    // Find an extent to reduce unnecessary calls to RenjuAIUtils::remoteCell
    int min_r = INT_MAX, min_c = INT_MAX, max_r = INT_MIN, max_c = INT_MIN;
    for (int r = 0; r < g_board_size; ++r) {
        for (int c = 0; c < g_board_size; ++c) {
            if (gs[g_board_size * r + c] != 0) {
                if (r < min_r) min_r = r;
                if (c < min_c) min_c = c;
                if (r > max_r) max_r = r;
                if (c > max_c) max_c = c;
            }
        }
    }

    if (min_r - 2 < 0) min_r = 2;
    if (min_c - 2 < 0) min_c = 2;
    if (max_r + 2 >= g_board_size) max_r = g_board_size - 3;
    if (max_c + 2 >= g_board_size) max_c = g_board_size - 3;

    // Loop through all cells
    for (int r = min_r - 2; r <= max_r + 2; ++r) {
        for (int c = min_c - 2; c <= max_c + 2; ++c) {
            // Consider only empty cells
            if (gs[g_board_size * r + c] != 0) continue;

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
}

int RenjuAINegamax::negamax(char *gs, int player, int depth, int *move_r, int *move_c) {
    // Initialize with a minimum score
    int max_score = INT_MIN;

    // Eval game state
    if (depth == 0) return RenjuAIEval::evalState(gs, player);

    // Loop through all cells
    for (int r = 0; r < g_board_size; ++r) {
        for (int c = 0; c < g_board_size; ++c) {
            // Consider only empty cells
            if (RenjuAIUtils::getCell(gs, r, c) != 0) continue;

            // Skip remote cells (no pieces within 2 cells)
            if (RenjuAIUtils::remoteCell(gs, r, c)) continue;

            // Execute move
            RenjuAIUtils::setCell(gs, r, c, static_cast<char>(player));

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
