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
#include <climits>
#include <iostream>

// Define static members
std::vector<std::vector<RenjuAIEval::DirectionPattern>>
    *RenjuAIEval::preset_patterns = nullptr;

std::vector<int> *RenjuAIEval::preset_scores = nullptr;

int RenjuAIEval::evalState(const char *gs, int player) {
    // Check parameters
    if (gs == nullptr) return 0;

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

    // Generate preset patterns structure in memory
    if (preset_patterns == nullptr) generatePresetPatterns();

    int score = 0;
    for (bool contiguous = false;; contiguous = true) {
        auto adm = measureAllDirections(gs, r, c, player, contiguous);

        // Choose the better between contiguous and non-contiguous
        score = std::max(score, evalADM(adm));

        // Release ADM
        for (auto dm : *adm) delete dm;
        delete adm;

        if (contiguous == true) break;
    }
    return score;
}

int RenjuAIEval::evalADM(std::vector<DirectionMeasurement *> *all_direction_measurement) {
    int score = 0;
    int size = preset_patterns->size();
    for (int i = 0; i < size; i++) {
        score += matchPattern(all_direction_measurement, &(*preset_patterns)[i]) * (*preset_scores)[i];
    }
    return score;
}

int RenjuAIEval::matchPattern(std::vector<DirectionMeasurement *> *all_direction_measurement,
                              std::vector<DirectionPattern> *patterns) {
    // Check arguments
    if (all_direction_measurement == nullptr) return -1;
    if (patterns == nullptr) return -1;

    int match_count = INT_MAX, pattern_match_count = 0;

    for (auto p : *patterns) {
        // Initialize counter
        pattern_match_count = 0;

        for (auto dm : *all_direction_measurement) {
            if (dm->length == p.length && dm->cut_count == p.cut_count &&
                (p.space_count == -1 || dm->space_count == p.space_count)) {
                pattern_match_count++;
            }
        }
        if (pattern_match_count < p.min_occurrence) pattern_match_count = 0;
        match_count = std::min(match_count, pattern_match_count);
    }
    return match_count;
}

std::vector<RenjuAIEval::DirectionMeasurement *>
    *RenjuAIEval::measureAllDirections(const char *gs,
                                       int r,
                                       int c,
                                       int player,
                                       bool contiguous) {
    //
    if (gs == nullptr) return nullptr;
    if (r < 0 || r >= 15 || c < 0 || c >= 15) return nullptr;

    //
    auto result = new std::vector<DirectionMeasurement *>();

    //
    for (int dr = 0; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc <= 0) continue;
            DirectionMeasurement *dm = measureDirection(gs, r, c, dr, dc, player, contiguous);
            result->push_back(dm);
        }
    }

    return result;
}

RenjuAIEval::DirectionMeasurement *RenjuAIEval::measureDirection(const char *gs,
                                                                 int r, int c,
                                                                 int dr, int dc,
                                                                 int player,
                                                                 bool contiguous) {
    // Check arguments
    if (gs == nullptr) return nullptr;
    if (r < 0 || r >= 15 || c < 0 || c >= 15) return nullptr;
    if (dr == 0 && dc == 0) return nullptr;

    // Initialization
    int cr = r, cc = c;
    DirectionMeasurement *result = new DirectionMeasurement();
    result->length = 1, result->cut_count = 2, result->space_count = 0;

    int space_allowance = 1;
    if (contiguous) space_allowance = 0;

    for (bool reversed = false;; reversed = true) {
        while (true) {
            // Move
            cr += dr; cc += dc;

            // Get cell value
            int cell = RenjuAIUtils::getCell(gs, cr, cc);

            // Check if reaches edge
            if (cell < 0) break;

            // Empty cells
            if (cell == 0) {
                if (space_allowance > 0 && RenjuAIUtils::getCell(gs, cr + dr, cc + dc) == player) {
                    space_allowance--; result->space_count++;
                    continue;
                } else {
                    result->cut_count--;
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
        result->cut_count = 0;
    }

    return result;
}

void RenjuAIEval::generatePresetPatterns() {
    std::vector<std::vector<std::vector<int>>> patterns = {
        {{1, 5, 0,  0}},
        {{2, 4, 0, -1}},
        {{2, 4, 1, -1}},
        {{1, 4, 1, -1}, {1, 3, 0, -1}},
        {{1, 4, 0,  0}},
        {{1, 4, 0,  1}},
        {{1, 4, 1, -1}},
        {{2, 3, 0, -1}},
        {{1, 3, 0, -1}},
        {{2, 2, 0, -1}},
        {{1, 3, 1, -1}},
        {{1, 2, 0, -1}},
        {{1, 2, 1, -1}},
        {{1, 1, 0, -1}},
        {{1, 1, 1, -1}}
    };

    std::vector<int> scores = {
        100000,
        1000,
        1000,
        1000,
        1000,
        60,
        30,
        500,
        60,
        8,
        5,
        4,
        3,
        1,
        1
    };

    preset_patterns = new std::vector<std::vector<DirectionPattern>>();
    preset_scores = new std::vector<int>();

    for (auto pattern : patterns) {
        std::vector<DirectionPattern> p;
        for (auto dir_pattern : pattern) {
            DirectionPattern dp;
            dp.min_occurrence = dir_pattern[0];
            dp.length         = dir_pattern[1];
            dp.cut_count      = dir_pattern[2];
            dp.space_count    = dir_pattern[3];
            p.push_back(dp);
        }
        preset_patterns->push_back(p);
    }

    for (auto score : scores) {
        preset_scores->push_back(score);
    }
}

int RenjuAIEval::winningPlayer(const char *gs) {
    for (int r = 0; r < 15; r++) {
        for (int c = 0; c < 15; c++) {
            int cell = RenjuAIUtils::getCell(gs, r, c);
            if (cell == 0) continue;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc <= 0) continue;
                    auto dm = measureDirection(gs, r, c, dr, dc, cell, 1);
                    if (dm->length >= 5) {
                        delete dm;
                        return cell;
                    }
                    delete dm;
                }
            }
        }
    }
    return 0;
}

void RenjuAIEval::test(char *gs) {
    // Unit test
    RenjuAIUtils::setCell(gs, 2, 2, 1);
    RenjuAIUtils::setCell(gs, 2, 4, 1);
    RenjuAIUtils::setCell(gs, 2, 5, 1);
    RenjuAIUtils::setCell(gs, 3, 4, 1);
    DirectionMeasurement *m = measureDirection(gs, 2, 3, 0, 1, 1, false);
    std::cout << "len: " << m->length << std::endl;
    std::cout << "cut_count: " << m->cut_count << std::endl;
    std::cout << "space_count: " << m->space_count << std::endl;
    std::cout << "--" << std::endl;
    delete m;

    auto adm = measureAllDirections(gs, 2, 3, 1, false);
    for (auto dm : *adm) {
        std::cout << "len: " << dm->length << std::endl;
        std::cout << "cut_count: " << dm->cut_count << std::endl;
        std::cout << "space_count: " << dm->space_count << std::endl;
        std::cout << "--" << std::endl;
        delete dm;
    }
    delete adm;


    adm = measureAllDirections(gs, 2, 3, 1, false);
    DirectionPattern p;
    p.min_occurrence = 1;
    p.length = 4;
    p.cut_count = 0;
    p.space_count = -1;

    DirectionPattern p1;
    p1.min_occurrence = 1;
    p1.length = 2;
    p1.cut_count = 0;
    p1.space_count = -1;

    std::vector<DirectionPattern> patterns;
    patterns.push_back(p);
    patterns.push_back(p1);

    std::cout << matchPattern(adm, &patterns) << std::endl;

    std::cout << std::endl;

    std::cout << evalMove(gs, 2, 3, 1) << std::endl;

    std::cout << evalState(gs, 1) << std::endl;

    std::cout << RenjuAIUtils::remoteCell(gs, 7, 7) << std::endl;

    for (int i = 0; i < 20; i++)
        evalState(gs, 1);
}
