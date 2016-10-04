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

#ifndef INCLUDE_AI_EVAL_H_
#define INCLUDE_AI_EVAL_H_

#define RENJU_AI_EVAL_WINNING_SCORE 10000
#define RENJU_AI_EVAL_THREATENING_SCORE 30

class RenjuAIEval {
 public:
    RenjuAIEval();
    ~RenjuAIEval();

    // Evaluate the entire game state as a player
    static int evalState(const char *gs, int player);

    // Evaluate one possible move as a player
    static int evalMove(const char *gs, int r, int c, int player);

    // Check if any player is winning based on a given state
    static int winningPlayer(const char *gs);

    // Unit tests
    static void test(char *gs);

// Allow testing private members in this class
#ifndef RENJU_PARALLEL_TEST
 private:
#endif
    // Result of a single direction measurement
    struct DirectionMeasurement {
        char length;          // Number of pieces in a row
        char block_count;     // Number of ends blocked by edge or the other player (0-2)
        char space_count;     // Number of spaces in the middle of pattern
    };

    // A single direction pattern
    struct DirectionPattern {
        char min_occurrence;  // Minimum number of occurrences to match
        char length;          // Length of pattern (pieces in a row)
        char block_count;     // Number of ends blocked by edge or the other player (0-2)
        char space_count;     // Number of spaces in the middle of pattern (-1: Ignore value)
    };

    // An array of preset patterns
    static DirectionPattern *preset_patterns;

    // Preset scores of each preset pattern
    static int *preset_scores;

    // Loads preset patterns into memory
    static void generatePresetPatterns(DirectionPattern **preset_patterns,
                                       int **preset_scores,
                                       int *preset_patterns_size);

    // Evaluates an all-direction measurement
    static int evalADM(DirectionMeasurement *all_direction_measurement);

    // Tries to match a set of patterns with an all-direction measurement
    static int matchPattern(DirectionMeasurement *all_direction_measurement,
                            DirectionPattern *patterns);

    // Measures all 4 directions
    static void measureAllDirections(const char *gs,
                                     int r,
                                     int c,
                                     int player,
                                     bool consecutive,
                                     RenjuAIEval::DirectionMeasurement *adm);

    // Measure a single direction
    static void measureDirection(const char *gs,
                                 int r, int c,
                                 int dr, int dc,
                                 int player,
                                 bool consecutive,
                                 RenjuAIEval::DirectionMeasurement *result);
};

#endif  // INCLUDE_AI_EVAL_H_
