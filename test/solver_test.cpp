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

namespace sudoku {

// Test helper constants/functions.
namespace solverTest {

const int subrows = 2, subcols = 2;
const std::vector<int> initVec = {
    1, 0,  0, 0,
    0, 0,  1, 0,

    0, 4,  0, 0,
    0, 0,  0, 3,
};
const std::vector<int> solutionVec = {
    1, 2,  3, 4,
    4, 3,  1, 2,

    3, 4,  2, 1,
    2, 1,  4, 3,
};

const Grid initGrid(subrows, subcols, initVec);
const Grid solutionGrid(subrows, subcols, solutionVec);

Solver init() {
  return Solver(initGrid);
}

void checkMoves(Solver& solver, int row, int col, int val, bool contains) {
  for (int i = 0; i < solver.getGrid().getCols(); ++i) {
    if (i == col)
      continue;
    if (contains) {
      EXPECT_THAT(solver.getMoves(row, i), Contains(val));
    } else {
      EXPECT_THAT(solver.getMoves(row, i), Each(Ne(val)));
    }
  }

  for (int i = 0; i < solver.getGrid().getRows(); ++i) {
    if (i == row)
      continue;
    if (contains) {
      EXPECT_THAT(solver.getMoves(i, col), Contains(val));
    } else {
      EXPECT_THAT(solver.getMoves(i, col), Each(Ne(val)));
    }
  }

  const int istart = (row / solverTest::subrows) * solverTest::subrows;
  const int jstart = (col / solverTest::subcols) * solverTest::subcols;
  for (int i = istart; i < istart + solverTest::subrows; i++) {
    for (int j = jstart; j < jstart + solverTest::subcols; j++) {
      if (contains) {
        EXPECT_THAT(solver.getMoves(i, j), Contains(val));
      } else {
        EXPECT_THAT(solver.getMoves(i, j), Each(Ne(val)));
      }
    }
  }
}

} /* namespace solverTest */

// Tests for the Solver class.

// Tests constructor with initGrid argument.
TEST(Solver, constructorOneArg) {
  Solver solver = solverTest::init();

  EXPECT_EQ(solver.getGrid().getValues(), solverTest::initGrid.getValues());
  EXPECT_EQ(solver.getMovesVec().size(), solverTest::initGrid.getSize());
  EXPECT_EQ(solver.getHistoryVec().size(), solverTest::initGrid.getSize());
}

// Tests the originally set at (row, col) method.
TEST(Solver, isOrigSet) {
  Solver solver = solverTest::init();

  for (int i = 0; i < solver.getGrid().getRows(); ++i) {
    for (int j = 0; j < solver.getGrid().getCols(); ++j) {
      int index = solver.getGrid().getIndex(i, j);
      EXPECT_EQ(solver.isOrigSet(i, j), (solverTest::initVec[index] != 0));
    }
  }
}

// Tests the get moves at (row, col) method.
TEST(Solver, getMoves) {
  Solver solver = solverTest::init();

  unordered_set<int> result = solver.getMoves(1, 1);
  unordered_set<int> actual({2, 3});
  EXPECT_EQ(result, actual);
}

// Tests the get history at (row, col) method.
TEST(Solver, getHistory) {
  Solver solver = solverTest::init();

  ASSERT_EQ(solver.assign(1, 1, 2), true);

  vector<int> history = solver.getHistory(1, 1);

  unordered_set<int> result(history.begin(), history.end());
  unordered_set<int> actual({0, 1, 4, 5, 6, 7, 9, 13});

  EXPECT_EQ(history.size(), actual.size());
  EXPECT_THAT(result, ContainerEq(actual));
}

// Tests the assign value to (row, col) method.
TEST(Solver, assignValid) {
  const int row = 2, col = 2, val = 2;
  Solver solver = solverTest::init();
  int prevNumMoves = solver.getMoves(row, col).size();

  EXPECT_EQ(solver.assign(row, col, val), true);
  EXPECT_EQ(solver.getGrid().getValue(row, col), val);
  EXPECT_LT(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_LT(0, solver.getHistory(row, col).size());

  solverTest::checkMoves(solver, row, col, val, false);
}

TEST(Solver, assignInvalidRow) {
  const int row = 2, col = 2, val = 4;
  Solver solver = solverTest::init();
  int prevNumMoves = solver.getMoves(row, col).size();

  EXPECT_EQ(solver.assign(row, col, val), false);
  EXPECT_EQ(solver.getGrid().getValue(row, col), 0);
  EXPECT_EQ(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_EQ(solver.getHistory(row, col).size(), 0);
}

TEST(Solver, assignInvalidCol) {
  const int row = 2, col = 2, val = 1;
  Solver solver = solverTest::init();
  int prevNumMoves = solver.getMoves(row, col).size();

  EXPECT_EQ(solver.assign(row, col, val), false);
  EXPECT_EQ(solver.getGrid().getValue(row, col), 0);
  EXPECT_EQ(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_EQ(solver.getHistory(row, col).size(), 0);
}

TEST(Solver, assignInvalidSubgrid) {
  const int row = 2, col = 2, val = 3;
  Solver solver = solverTest::init();
  int prevNumMoves = solver.getMoves(row, col).size();

  EXPECT_EQ(solver.assign(row, col, val), false);
  EXPECT_EQ(solver.getGrid().getValue(row, col), 0);
  EXPECT_EQ(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_EQ(solver.getHistory(row, col).size(), 0);
}

TEST(Solver, assignInvalidOccupied) {
  const int row = 2, col = 2, val = 1;
  Solver solver = solverTest::init();
  int prevValue = solver.getGrid().getValue(row, col);
  int prevNumMoves = solver.getMoves(row, col).size();
  int prevNumHistory = solver.getHistory(row, col).size();

  EXPECT_EQ(solver.assign(row, col, val), false);
  EXPECT_EQ(solver.getGrid().getValue(row, col), prevValue);
  EXPECT_EQ(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_EQ(solver.getHistory(row, col).size(), prevNumHistory);
}

// Tests the unassign value at (row, col) method.
TEST(Solver, unassignValid) {
  const int row = 2, col = 2, val = 2;
  Solver solver = solverTest::init();
  int prevValue = solver.getGrid().getValue(row, col);
  int prevNumMoves = solver.getMoves(row, col).size();
  int prevNumHistory = solver.getHistory(row, col).size();

  ASSERT_EQ(solver.assign(row, col, val), true);

  EXPECT_EQ(solver.unassign(row, col), true);
  EXPECT_EQ(solver.getGrid().getValue(row, col), prevValue);
  EXPECT_EQ(solver.getMoves(row, col).size(), prevNumMoves);
  EXPECT_EQ(solver.getHistory(row, col).size(), prevNumHistory);

  solverTest::checkMoves(solver, row, col, val, true);
}

TEST(Solver, unassignInvalid) {
  const int row = 2, col = 2, val = 2;
  Solver solver = solverTest::init();
  ASSERT_EQ(solver.assign(row, col, val), true);
  EXPECT_EQ(solver.unassign(0, 0), false);
}

// Tests the solve method.
TEST(Solver, solve) {
  Solver solver = solverTest::init();

  EXPECT_LT(0, solver.solve());
  EXPECT_THAT(solver.getGrid().getValues(), ContainerEq(solverTest::solutionVec));
}

// Tests the reset method.
TEST(Solver, reset) {
  const int row = 2, col = 2, val = 2;
  Solver solver = solverTest::init();

  const vector<unordered_set<int>> prevMoves = solver.getMovesVec();
  const vector<vector<int>> prevHistory = solver.getHistoryVec();

  ASSERT_EQ(solver.assign(row, col, val), true);

  solver.reset();
  EXPECT_EQ(solver.getGrid().getValues(), solverTest::initGrid.getValues());
  EXPECT_EQ(solver.getMovesVec(), prevMoves);
  EXPECT_EQ(solver.getHistoryVec(), prevHistory);
}

} /* namespace sudoku */
