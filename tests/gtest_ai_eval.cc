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

class RenjuAIEvalTest : public ::testing::Test {
protected:
    char gs[255] = {0};
};

TEST_F(RenjuAIEvalTest, meausreDirection) {
    RenjuAIEval::DirectionMeasurement dm;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(true, dm.length == 1 && dm.block_count == 1 && dm.space_count == 0);

    // * 0 0
    // 0 1 0
    // 0 0 0
    gs[1 * 15 + 1] = 1;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(true, dm.length == 2 && dm.block_count == 1 && dm.space_count == 0);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 0
    gs[2 * 15 + 2] = 1;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(true, dm.length == 3 && dm.block_count == 1 && dm.space_count == 0);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 1 0
    // 0 0 0 2
    gs[3 * 15 + 3] = 2;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(true, dm.length == 3 && dm.block_count == 2 && dm.space_count == 0);

    // * 0 0 0
    // 0 1 0 0
    // 0 0 0 0
    // 0 0 0 1
    gs[2 * 15 + 2] = 0;
    gs[3 * 15 + 3] = 1;
    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, true, &dm);
    EXPECT_EQ(true, dm.length == 2 && dm.block_count == 1 && dm.space_count == 0);

    RenjuAIEval::measureDirection(gs, 0, 0, 1, 1, 1, false, &dm);
    EXPECT_EQ(true, dm.length == 3 && dm.block_count == 1 && dm.space_count == 1);
}

TEST_F(RenjuAIEvalTest, winningPlayer) {
    EXPECT_EQ(0, RenjuAIEval::winningPlayer(gs));

    gs[2] = 1; gs[3] = 1; gs[4] = 1; gs[5] = 1;
    EXPECT_EQ(0, RenjuAIEval::winningPlayer(gs));

    gs[6] = 1;
    EXPECT_EQ(1, RenjuAIEval::winningPlayer(gs));

    gs[7] = 1;
    EXPECT_EQ(1, RenjuAIEval::winningPlayer(gs));

    memset(gs, 0, 225);

    gs[2] = 1; gs[3] = 2; gs[4] = 2; gs[5] = 2; gs[6] = 2; gs[7] = 2;
    EXPECT_EQ(2, RenjuAIEval::winningPlayer(gs));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
