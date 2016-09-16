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

#include <vector>

class RenjuAIEval {
 public:
    RenjuAIEval();
    ~RenjuAIEval();

    static int evalState(const char *gs, int player);
    static int evalMove(const char *gs, int r, int c, int player);
    static int winningPlayer(const char *gs);
    static void test(char *gs);

 private:
    struct DirectionMeasurement {
        int length;
        int cut_count;
        int space_count;
    };

    struct DirectionPattern {
        int min_occurrence;
        int length;
        int cut_count;
        int space_count;
    };

    static std::vector<std::vector<DirectionPattern>> *preset_patterns;
    static std::vector<int> *preset_scores;

    static void generatePresetPatterns();
    static int evalADM(std::vector<DirectionMeasurement *> *all_direction_measurement);
    static int matchPattern(std::vector<DirectionMeasurement *> *all_direction_measurement,
                            std::vector<DirectionPattern> *patterns);

    static std::vector<DirectionMeasurement *> *measureAllDirections(const char *gs,
                                                                     int r,
                                                                     int c,
                                                                     int player,
                                                                     bool contiguous);

    static DirectionMeasurement *measureDirection(const char *gs,
                                                  int r, int c,
                                                  int dr, int dc,
                                                  int player,
                                                  bool contiguous);

};

#endif  // INCLUDE_AI_EVAL_H_
