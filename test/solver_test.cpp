/*
 * solver_test.cpp
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <src/solver.h>

using testing::Each;
using testing::Ne;
using testing::Contains;
using testing::ContainerEq;

using std::vector;
using std::unordered_set;

namespace sudoku {

// Test helper constants/functions.
namespace solverTest {

const int subrows = 3, subcols = 3;
const std::vector<int> initVec = {
    0, 0, 5,  3, 0, 0,  0, 0, 0,
    8, 0, 0,  0, 0, 0,  0, 2, 0,
    0, 7, 0,  0, 1, 0,  5, 0, 0,

    4, 0, 0,  0, 0, 5,  3, 0, 0,
    0, 1, 0,  0, 7, 0,  0, 0, 6,
    0, 0, 3,  2, 0, 0,  0, 8, 0,

    0, 6, 0,  5, 0, 0,  0, 0, 9,
    0, 0, 4,  0, 0, 0,  0, 3, 0,
    0, 0, 0,  0, 0, 9,  7, 0, 0,
};
const std::vector<int> solutionVec = {
    1, 4, 5,  3, 2, 7,  6, 9, 8,
    8, 3, 9,  6, 5, 4,  1, 2, 7,
    6, 7, 2,  9, 1, 8,  5, 4, 3,

    4, 9, 6,  1, 8, 5,  3, 7, 2,
    2, 1, 8,  4, 7, 3,  9, 5, 6,
    7, 5, 3,  2, 9, 6,  4, 8, 1,

    3, 6, 7,  5, 4, 2,  8, 1, 9,
    9, 8, 4,  7, 6, 1,  2, 3, 5,
    5, 2, 1,  8, 3, 9,  7, 6, 4,
};

const Grid initGrid(subrows, subcols, initVec);
const Grid solutionGrid(subrows, subcols, solutionVec);

Solver init(VariableSortType varSortType, ValueSortType valSortType) {
  return Solver(initGrid, varSortType, valSortType);
}

} /* namespace solverTest */

// Tests for the Solver class.

// Tests constructor with initGrid argument.
TEST(Solver, constructorOneArg) {
  Solver solver = solverTest::init(VariableSortType::NONE,
      ValueSortType::NONE);

  EXPECT_EQ(solver.getGrid().getValues(), solverTest::initGrid.getValues());
}

// Tests the solve method.
TEST(Solver, solveNoneNone) {
  Solver solver = solverTest::init(VariableSortType::NONE,
      ValueSortType::NONE);
  Grid solution;
  int steps = 0;

  EXPECT_TRUE(solver.solve(solution, steps));
  EXPECT_EQ(solution.getValues(), solverTest::solutionGrid.getValues());
  EXPECT_GE(steps, 1);
}

TEST(Solver, solveRandom) {
  Solver solver = solverTest::init(VariableSortType::RANDOMIZED,
      ValueSortType::RANDOMIZED);
  Grid solution;
  int steps = 0;

  EXPECT_TRUE(solver.solve(solution, steps));
  EXPECT_EQ(solution.getValues(), solverTest::solutionGrid.getValues());
  EXPECT_GE(steps, 1);
}

TEST(Solver, solveOpt) {
  Solver solver = solverTest::init(VariableSortType::MOST_CONSTRAINED,
      ValueSortType::LEAST_CONSTRAINING);
  Grid solution;
  int steps = 0;

  EXPECT_TRUE(solver.solve(solution, steps));
  EXPECT_EQ(solution.getValues(), solverTest::solutionGrid.getValues());
  EXPECT_GE(steps, 1);
}

} /* namespace sudoku */
