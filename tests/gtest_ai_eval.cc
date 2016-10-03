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

#include <gtest/gtest.h>
#include <ai/eval.h>
#include <ai/utils.h>

class RenjuAIEvalTest : public ::testing::Test {
 protected:
    char gs[361] = {0};
};

TEST_F(RenjuAIEvalTest, winningPlayer) {
    EXPECT_EQ(0, RenjuAIEval::winningPlayer(gs));

    gs[2] = 1; gs[3] = 1; gs[4] = 1; gs[5] = 1;
    EXPECT_EQ(0, RenjuAIEval::winningPlayer(gs));

    gs[6] = 1;
    EXPECT_EQ(1, RenjuAIEval::winningPlayer(gs));

    gs[7] = 1;
    EXPECT_EQ(1, RenjuAIEval::winningPlayer(gs));

    memset(gs, 0, 361);

    gs[2] = 1; gs[3] = 2; gs[4] = 2; gs[5] = 2; gs[6] = 2; gs[7] = 2;
    EXPECT_EQ(2, RenjuAIEval::winningPlayer(gs));
}

TEST_F(RenjuAIEvalTest, meausreDirection) {
    RenjuAIEval::DirectionMeasurement dm;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(1, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // * 0 0
    // 0 1 0
    // 0 0 0
    RenjuAIUtils::setCell(gs, 1, 1, 1);
    gs[1 * 15 + 1] = 1;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(2, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 0
    RenjuAIUtils::setCell(gs, 2, 2, 1);
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(3, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 2
    RenjuAIUtils::setCell(gs, 3, 3, 2);
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(3, dm.length); EXPECT_EQ(2, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 0 0
    // 0 0 0 1
    RenjuAIUtils::setCell(gs, 2, 2, 0);
    RenjuAIUtils::setCell(gs, 3, 3, 1);
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(2, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(0, dm.space_count);

    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, false, &dm);
    EXPECT_EQ(3, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(1, dm.space_count);

    // 0 0 0 0 0
    // 0 1 * 1 0
    // 0 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 1, 1);
    RenjuAIUtils::setCell(gs, 1, 3, 1);
    RenjuAIEval::measureDirection(gs, 1, 2, 0, 1, 1, true, &dm);
    EXPECT_EQ(3, dm.length); EXPECT_EQ(0, dm.block_count); EXPECT_EQ(0, dm.space_count);

    RenjuAIEval::measureDirection(gs, 1, 2, 0, 1, 1, false, &dm);
    EXPECT_EQ(3, dm.length); EXPECT_EQ(0, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // 0 0 0 0 0 0
    // 1 1 * 1 0 0
    // 0 0 0 0 0 0
    RenjuAIUtils::setCell(gs, 1, 0, 1);
    RenjuAIEval::measureDirection(gs, 1, 2, 0, 1, 1, true, &dm);
    EXPECT_EQ(4, dm.length); EXPECT_EQ(1, dm.block_count); EXPECT_EQ(0, dm.space_count);

    // 0 0 0 0 0 0 0
    // 0 1 * 1 0 1 0
    // 0 0 0 0 0 0 0
    RenjuAIUtils::setCell(gs, 1, 0, 0);
    RenjuAIUtils::setCell(gs, 1, 5, 1);
    RenjuAIEval::measureDirection(gs, 1, 2, 0, 1, 1, false, &dm);
    EXPECT_EQ(4, dm.length); EXPECT_EQ(0, dm.block_count); EXPECT_EQ(1, dm.space_count);
}

TEST_F(RenjuAIEvalTest, meausreAllDirections) {
    RenjuAIEval::DirectionMeasurement adm[4];

    // * 0
    // 0 0
    RenjuAIEval::measureAllDirections(gs, 0, 0, 1, true, adm);
    EXPECT_EQ(1, adm[0].length); EXPECT_EQ(1, adm[1].length); EXPECT_EQ(1, adm[2].length); EXPECT_EQ(1, adm[3].length);
    EXPECT_EQ(1, adm[0].block_count); EXPECT_EQ(1, adm[1].block_count); EXPECT_EQ(1, adm[2].block_count); EXPECT_EQ(2, adm[3].block_count);
    EXPECT_EQ(0, adm[0].space_count); EXPECT_EQ(0, adm[1].space_count); EXPECT_EQ(0, adm[2].space_count); EXPECT_EQ(0, adm[3].space_count);

    // 0 0 0
    // * 0 0
    // 0 0 0
    RenjuAIEval::measureAllDirections(gs, 1, 0, 1, true, adm);
    EXPECT_EQ(1, adm[0].length); EXPECT_EQ(1, adm[1].length); EXPECT_EQ(1, adm[2].length); EXPECT_EQ(1, adm[3].length);
    EXPECT_EQ(1, adm[0].block_count); EXPECT_EQ(1, adm[1].block_count); EXPECT_EQ(0, adm[2].block_count); EXPECT_EQ(1, adm[3].block_count);
    EXPECT_EQ(0, adm[0].space_count); EXPECT_EQ(0, adm[1].space_count); EXPECT_EQ(0, adm[2].space_count); EXPECT_EQ(0, adm[3].space_count);

    // 0 0 0
    // * 1 0
    // 0 0 0
    RenjuAIUtils::setCell(gs, 1, 1, 1);
    RenjuAIEval::measureAllDirections(gs, 1, 0, 1, true, adm);
    EXPECT_EQ(2, adm[0].length); EXPECT_EQ(1, adm[1].length); EXPECT_EQ(1, adm[2].length); EXPECT_EQ(1, adm[3].length);
    EXPECT_EQ(1, adm[0].block_count); EXPECT_EQ(1, adm[1].block_count); EXPECT_EQ(0, adm[2].block_count); EXPECT_EQ(1, adm[3].block_count);
    EXPECT_EQ(0, adm[0].space_count); EXPECT_EQ(0, adm[1].space_count); EXPECT_EQ(0, adm[2].space_count); EXPECT_EQ(0, adm[3].space_count);

    // 0 0 0
    // 0 2 0
    // 0 2 0
    // 0 2 0
    // 0 * 0
    // 0 0 0
    RenjuAIUtils::setCell(gs, 1, 1, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 3, 1, 2);
    RenjuAIEval::measureAllDirections(gs, 4, 1, 2, true, adm);
    EXPECT_EQ(1, adm[0].length); EXPECT_EQ(1, adm[1].length); EXPECT_EQ(4, adm[2].length); EXPECT_EQ(1, adm[3].length);
    EXPECT_EQ(0, adm[0].block_count); EXPECT_EQ(0, adm[1].block_count); EXPECT_EQ(0, adm[2].block_count); EXPECT_EQ(0, adm[3].block_count);
    EXPECT_EQ(0, adm[0].space_count); EXPECT_EQ(0, adm[1].space_count); EXPECT_EQ(0, adm[2].space_count); EXPECT_EQ(0, adm[3].space_count);
}

TEST_F(RenjuAIEvalTest, matchPattern) {

    RenjuAIEval::DirectionPattern *preset_patterns = nullptr;
    int *preset_scores = nullptr;
    int preset_patterns_size = 0;
    RenjuAIEval::generatePresetPatterns(&preset_patterns, &preset_scores, &preset_patterns_size);

    RenjuAIEval::DirectionMeasurement adm[4];

    // 0 0 0
    // 0 2 0
    // 0 2 0
    // 0 2 0
    // 0 * 0
    // 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 1, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 3, 1, 2);
    RenjuAIEval::measureAllDirections(gs, 4, 1, 2, true, adm);
    EXPECT_EQ(1, RenjuAIEval::matchPattern(adm, &preset_patterns[2]));

    // 0 0 0 0
    // 0 * 2 2
    // 0 2 0 0
    // 0 0 0 0
    // 0 2 0 0
    // 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 2, 2);
    RenjuAIUtils::setCell(gs, 1, 3, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 4, 1, 2);
    RenjuAIEval::measureAllDirections(gs, 1, 1, 2, false, adm);
    EXPECT_EQ(1, RenjuAIEval::matchPattern(adm, &preset_patterns[14]));

    // 0 0 0 0 0
    // 0 * 2 2 0
    // 0 0 2 0 0
    // 0 0 0 2 0
    // 0 0 0 0 0
    // 0 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 2, 2);
    RenjuAIUtils::setCell(gs, 1, 3, 2);
    RenjuAIUtils::setCell(gs, 2, 2, 2);
    RenjuAIUtils::setCell(gs, 3, 3, 2);
    RenjuAIEval::measureAllDirections(gs, 1, 1, 2, false, adm);
    EXPECT_EQ(1, RenjuAIEval::matchPattern(adm, &preset_patterns[14]));

    // 0 0 0 0 0 0
    // 0 * 2 0 2 0
    // 0 2 0 0 0 0
    // 0 2 0 0 0 0
    // 0 2 0 0 0 0
    // 0 1 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 2, 2);
    RenjuAIUtils::setCell(gs, 1, 4, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 3, 1, 2);
    RenjuAIUtils::setCell(gs, 4, 1, 2);
    RenjuAIUtils::setCell(gs, 5, 1, 1);
    RenjuAIEval::measureAllDirections(gs, 1, 1, 2, false, adm);
    EXPECT_EQ(1, RenjuAIEval::matchPattern(adm, &preset_patterns[10]));
}

TEST_F(RenjuAIEvalTest, evalMove) {

    // 0 0 0
    // 0 2 0
    // 0 2 0
    // 0 2 0
    // 0 * 0
    // 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 1, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 3, 1, 2);
    EXPECT_EQ(1000, RenjuAIEval::evalMove(gs, 4, 1, 2));

    // 0 0 0 0 0 0
    // 0 * 2 0 2 0
    // 0 2 0 0 0 0
    // 0 2 0 0 0 0
    // 0 2 0 0 0 0
    // 0 1 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 2, 2);
    RenjuAIUtils::setCell(gs, 1, 4, 2);
    RenjuAIUtils::setCell(gs, 2, 1, 2);
    RenjuAIUtils::setCell(gs, 3, 1, 2);
    RenjuAIUtils::setCell(gs, 4, 1, 2);
    RenjuAIUtils::setCell(gs, 5, 1, 1);
    EXPECT_EQ(700, RenjuAIEval::evalMove(gs, 1, 1, 2));

    // 0 0 0 0 0 0
    // 0 * 1 1 1 2
    // 0 0 0 0 0 0
    // 0 0 0 1 0 0
    // 0 0 0 0 1 0
    // 0 0 0 0 0 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 1, 2, 1);
    RenjuAIUtils::setCell(gs, 1, 3, 1);
    RenjuAIUtils::setCell(gs, 1, 4, 1);
    RenjuAIUtils::setCell(gs, 1, 5, 2);
    RenjuAIUtils::setCell(gs, 3, 3, 1);
    RenjuAIUtils::setCell(gs, 4, 4, 1);
    EXPECT_EQ(700, RenjuAIEval::evalMove(gs, 1, 1, 1));

    // 2 0 0 0 0 0 0
    // 0 * 1 0 1 0 2
    // 0 0 1 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 1 0 0
    // 0 0 0 0 0 1 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 0, 0, 2);
    RenjuAIUtils::setCell(gs, 1, 2, 1);
    RenjuAIUtils::setCell(gs, 1, 4, 1);
    RenjuAIUtils::setCell(gs, 1, 6, 2);
    RenjuAIUtils::setCell(gs, 2, 2, 1);
    RenjuAIUtils::setCell(gs, 4, 4, 1);
    RenjuAIUtils::setCell(gs, 5, 5, 1);
    EXPECT_EQ(700, RenjuAIEval::evalMove(gs, 1, 1, 1));

    // 2 0 0 0 0 0 0
    // 0 * 1 0 1 0 2
    // 0 0 1 0 0 0 0
    // 0 0 0 0 0 0 0
    // 0 0 0 0 1 0 0
    // 0 0 0 0 0 1 0
    memset(gs, 0, 361);
    RenjuAIUtils::setCell(gs, 0, 0, 2);
    RenjuAIUtils::setCell(gs, 1, 2, 1);
    RenjuAIUtils::setCell(gs, 1, 4, 1);
    RenjuAIUtils::setCell(gs, 1, 6, 2);
    RenjuAIUtils::setCell(gs, 2, 2, 1);
    RenjuAIUtils::setCell(gs, 4, 4, 1);
    RenjuAIUtils::setCell(gs, 5, 5, 1);
    EXPECT_EQ(700, RenjuAIEval::evalMove(gs, 1, 1, 1));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
