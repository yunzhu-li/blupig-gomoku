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

#include <ai/eval.h>
#include <ai/utils.h>
#include <utils/globals.h>
#include <stdlib.h>
#include <climits>
#include <cstring>

// Initialize global variables
RenjuAIEval::DirectionPattern *RenjuAIEval::preset_patterns = nullptr;
int *RenjuAIEval::preset_scores = nullptr;
int preset_patterns_size = 0;

int RenjuAIEval::evalState(const char *gs, int player) {
    // Check parameters
    if (gs == nullptr) return 0;

    // Evaluate all possible moves
    int score = 0;
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            score += evalMove(gs, r, c, player);
        }
    }
    return score;
}

int RenjuAIEval::evalMove(const char *gs, int r, int c, int player) {
    // Check parameters
    if (gs == nullptr) return 0;

    // Count evaluations
    g_eval_count++;

    // Generate preset patterns structure in memory
    if (preset_patterns == nullptr) {
        generatePresetPatterns(&preset_patterns, &preset_scores, &preset_patterns_size);
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
        if (!consecutive) score *= 0.9;

        // Choose the better between consecutive and non-consecutive
        max_score = std::max(max_score, score);

        if (consecutive) break;
    }
    return max_score;
}

int RenjuAIEval::evalADM(DirectionMeasurement *all_direction_measurement) {
    int score = 0;
    int size = preset_patterns_size;

    // Loop through and try to match all preset patterns
    for (int i = 0; i < size; i++) {
        score += matchPattern(all_direction_measurement, &preset_patterns[2 * i]) * preset_scores[i];
    }
    return score;
}

int RenjuAIEval::matchPattern(DirectionMeasurement *all_direction_measurement,
                              DirectionPattern *patterns) {
    // Check arguments
    if (all_direction_measurement == nullptr) return -1;
    if (patterns == nullptr) return -1;

    // Initialize match_count to INT_MAX since minimum value will be output
    int match_count = INT_MAX, single_pattern_match = 0;

    // Currently allows maximum 2 patterns
    for (int i = 0; i < 2; i++) {
        auto p = patterns[i];
        if (p.length == 0) break;

        // Initialize counter
        single_pattern_match = 0;

        // Loop through 4 directions
        for (int j = 0; j < 4; j++) {
            auto dm = all_direction_measurement[j];

            // Requires exact match
            if (dm.length == p.length && dm.block_count == p.block_count &&
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
    if (r < 0 || r >= 15 || c < 0 || c >= 15) return;

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
    if (r < 0 || r >= 15 || c < 0 || c >= 15) return;
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
            if (cr < 0 || cr >= 15 || cc < 0 || cc >= 15) break;

            // Get cell value
            int cell = gs[15 * cr + cc];

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
    if (result->length >= 5 && result->space_count == 0) {
        result->length = 5;
        result->block_count = 0;
    }
}

void RenjuAIEval::generatePresetPatterns(DirectionPattern **preset_patterns,
                                         int **preset_scores,
                                         int *preset_patterns_size) {
    DirectionPattern patterns[30] = {
        {1, 5, 0,  0}, {0, 0, 0,  0},
        {2, 4, 0, -1}, {0, 0, 0,  0},
        {2, 4, 1, -1}, {0, 0, 0,  0},
        {1, 4, 1, -1}, {1, 3, 0, -1},
        {1, 4, 0,  0}, {0, 0, 0,  0},
        {1, 4, 0,  1}, {0, 0, 0,  0},
        {1, 4, 1, -1}, {0, 0, 0,  0},
        {2, 3, 0, -1}, {0, 0, 0,  0},
        {1, 3, 0, -1}, {0, 0, 0,  0},
        {2, 2, 0, -1}, {0, 0, 0,  0},
        {1, 3, 1, -1}, {0, 0, 0,  0},
        {1, 2, 0, -1}, {0, 0, 0,  0},
        {1, 2, 1, -1}, {0, 0, 0,  0},
        {1, 1, 0, -1}, {0, 0, 0,  0},
        {1, 1, 1, -1}, {0, 0, 0,  0}
    };

    int scores[15] = {
        100000,
        1000,
        1000,
        1000,
        1000,
        50,
        30,
        500,
        60,
        30,
        5,
        4,
        3,
        1,
        1
    };

    *preset_patterns = (DirectionPattern *)malloc(sizeof(DirectionPattern) * 30);
    *preset_scores   = (int *)malloc(sizeof(int) * 15);

    memcpy(*preset_patterns, patterns, sizeof(DirectionPattern) * 30);
    memcpy(*preset_scores, scores, sizeof(int) * 15);

    *preset_patterns_size = 15;
}

int RenjuAIEval::winningPlayer(const char *gs) {
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            int cell = gs[15 * r + c];
            if (cell == 0) continue;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
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
