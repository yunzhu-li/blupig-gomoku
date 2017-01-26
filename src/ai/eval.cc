/*
 * blupig
 * Copyright (C) 2016-2017 Yunzhu Li
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

#include <ai/eval.h>
#include <ai/utils.h>
#include <utils/globals.h>
#include <stdlib.h>
#include <algorithm>
#include <climits>
#include <cstring>

// Initialize global variables
RenjuAIEval::DirectionPattern *RenjuAIEval::preset_patterns = nullptr;
int *RenjuAIEval::preset_scores = nullptr;
int preset_patterns_size = 0;
int preset_patterns_skip[6] = {0};

int RenjuAIEval::evalState(const char *gs, int player) {
    // Check parameters
    if (gs == nullptr ||
        player < 1 || player > 2) return 0;

    // Evaluate all possible moves
    int score = 0;
    for (int r = 0; r < g_board_size; ++r) {
        for (int c = 0; c < g_board_size; ++c) {
            score += evalMove(gs, r, c, player);
        }
    }
    return score;
}

int RenjuAIEval::evalMove(const char *gs, int r, int c, int player) {
    // Check parameters
    if (gs == nullptr ||
        player < 1 || player > 2) return 0;

    // Count evaluations
    ++g_eval_count;

    // Generate preset patterns structure in memory
    if (preset_patterns == nullptr) {
        generatePresetPatterns(&preset_patterns, &preset_scores, &preset_patterns_size, preset_patterns_skip);
    }

    // Allocate 4 direction measurements
    DirectionMeasurement adm[4];

    // Measure in consecutive and non-consecutive conditions
    int max_score = 0;
    for (bool consecutive = false;; consecutive = true) {
        // Execute measurement
        measureAllDirections(gs, r, c, player, consecutive, adm);

        int score = evalADM(adm);

        // Prefer consecutive
        // if (!consecutive) score *= 0.9;

        // Choose the better between consecutive and non-consecutive
        max_score = std::max(max_score, score);

        if (consecutive) break;
    }
    return max_score;
}

int RenjuAIEval::evalADM(DirectionMeasurement *all_direction_measurement) {
    int score = 0;
    int size = preset_patterns_size;

    // Add to score by length on each direction
    // Find the maximum length in ADM and skip some patterns
    int max_measured_len = 0;
    for (int i = 0; i < 4; i++) {
        int len = all_direction_measurement[i].length;
        max_measured_len = len > max_measured_len ? len : max_measured_len;
        score += len - 1;
    }
    int start_pattern = preset_patterns_skip[max_measured_len];

    // Loop through and try to match all preset patterns
    for (int i = start_pattern; i < size; ++i) {
        score += matchPattern(all_direction_measurement, &preset_patterns[2 * i]) * preset_scores[i];

        // Only match one threatening pattern
        if (score >= kRenjuAiEvalThreateningScore) break;
    }

    return score;
}

int RenjuAIEval::matchPattern(DirectionMeasurement *all_direction_measurement,
                              DirectionPattern *patterns) {
    // Check arguments
    if (all_direction_measurement == nullptr) return -1;
    if (patterns == nullptr) return -1;

    // Increment PM count
    g_pm_count++;

    // Initialize match_count to INT_MAX since minimum value will be output
    int match_count = INT_MAX, single_pattern_match = 0;

    // Currently allows maximum 2 patterns
    for (int i = 0; i < 2; ++i) {
        auto p = patterns[i];
        if (p.length == 0) break;

        // Initialize counter
        single_pattern_match = 0;

        // Loop through 4 directions
        for (int j = 0; j < 4; ++j) {
            auto dm = all_direction_measurement[j];

            // Requires exact match
            if (dm.length == p.length &&
                (p.block_count == -1 || dm.block_count == p.block_count) &&
                (p.space_count == -1 || dm.space_count == p.space_count)) {
                single_pattern_match++;
            }
        }

        // Consider minimum number of occurrences
        single_pattern_match /= p.min_occurrence;

        // Take smaller value
        match_count = match_count >= single_pattern_match ? single_pattern_match : match_count;
    }
    return match_count;
}

void RenjuAIEval::measureAllDirections(const char *gs,
                                       int r,
                                       int c,
                                       int player,
                                       bool consecutive,
                                       RenjuAIEval::DirectionMeasurement *adm) {
    // Check arguments
    if (gs == nullptr) return;
    if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return;

    // Measure 4 directions
    measureDirection(gs, r, c, 0,  1, player, consecutive, &adm[0]);
    measureDirection(gs, r, c, 1,  1, player, consecutive, &adm[1]);
    measureDirection(gs, r, c, 1,  0, player, consecutive, &adm[2]);
    measureDirection(gs, r, c, 1, -1, player, consecutive, &adm[3]);
}

void RenjuAIEval::measureDirection(const char *gs,
                                   int r, int c,
                                   int dr, int dc,
                                   int player,
                                   bool consecutive,
                                   RenjuAIEval::DirectionMeasurement *result) {
    // Check arguments
    if (gs == nullptr) return;
    if (r < 0 || r >= g_board_size || c < 0 || c >= g_board_size) return;
    if (dr == 0 && dc == 0) return;

    // Initialization
    int cr = r, cc = c;
    result->length = 1, result->block_count = 2, result->space_count = 0;

    int space_allowance = 1;
    if (consecutive) space_allowance = 0;

    for (bool reversed = false;; reversed = true) {
        while (true) {
            // Move
            cr += dr; cc += dc;

            // Validate position
            if (cr < 0 || cr >= g_board_size || cc < 0 || cc >= g_board_size) break;

            // Get cell value
            int cell = gs[g_board_size * cr + cc];

            // Empty cells
            if (cell == 0) {
                if (space_allowance > 0 && RenjuAIUtils::getCell(gs, cr + dr, cc + dc) == player) {
                    space_allowance--; result->space_count++;
                    continue;
                } else {
                    result->block_count--;
                    break;
                }
            }

            // Another player
            if (cell != player) break;

            // Current player
            result->length++;
        }

        // Reverse direction and continue (just once)
        if (reversed) break;
        cr = r; cc = c;
        dr = -dr; dc = -dc;
    }

    // More than 5 pieces in a row is equivalent to 5 pieces
    if (result->length >= 5) {
        if (result->space_count == 0) {
            result->length = 5;
            result->block_count = 0;
        } else {
            result->length = 4;
            result->block_count = 1;
        }
    }
}

void RenjuAIEval::generatePresetPatterns(DirectionPattern **preset_patterns,
                                         int **preset_scores,
                                         int *preset_patterns_size,
                                         int *preset_patterns_skip) {
    const int _size = 11;
    preset_patterns_skip[5] = 0;
    preset_patterns_skip[4] = 1;
    preset_patterns_skip[3] = 7;
    preset_patterns_skip[2] = 10;

    preset_patterns_skip[1] = _size;
    preset_patterns_skip[0] = _size;

    DirectionPattern patterns[_size * 2] = {
        {1, 5,  0,  0}, {0, 0,  0,  0},  // 10000
        {1, 4,  0,  0}, {0, 0,  0,  0},  // 700
        {2, 4,  1,  0}, {0, 0,  0,  0},  // 700
        {2, 4, -1,  1}, {0, 0,  0,  0},  // 700
        {1, 4,  1,  0}, {1, 4, -1,  1},  // 700
        {1, 4,  1,  0}, {1, 3,  0, -1},  // 500
        {1, 4, -1,  1}, {1, 3,  0, -1},  // 500
        {2, 3,  0, -1}, {0, 0,  0,  0},  // 300
        // {1, 4,  1,  0}, {0, 0,  0,  0},  // 1
        // {1, 4, -1,  1}, {0, 0,  0,  0},  // 1
        {3, 2,  0, -1}, {0, 0,  0,  0},  // 50
        {1, 3,  0, -1}, {0, 0,  0,  0},  // 20
        {1, 2,  0, -1}, {0, 0,  0,  0}   // 9
    };

    int scores[_size] = {
        10000,
        700,
        700,
        700,
        700,
        500,
        500,
        300,
        // 1,
        // 1,
        50,
        20,
        9
    };

    *preset_patterns = new DirectionPattern[_size * 2];
    *preset_scores   = new int[_size];

    memcpy(*preset_patterns, patterns, sizeof(DirectionPattern) * _size * 2);
    memcpy(*preset_scores, scores, sizeof(int) * _size);

    *preset_patterns_size = _size;
}

int RenjuAIEval::winningPlayer(const char *gs) {
    if (gs == nullptr) return 0;
    for (int r = 0; r < g_board_size; ++r) {
        for (int c = 0; c < g_board_size; ++c) {
            int cell = gs[g_board_size * r + c];
            if (cell == 0) continue;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc <= 0) continue;
                    DirectionMeasurement dm;
                    measureDirection(gs, r, c, dr, dc, cell, 1, &dm);
                    if (dm.length >= 5) return cell;
                }
            }
        }
    }
    return 0;
}
