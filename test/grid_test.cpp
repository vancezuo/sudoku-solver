/*
 * grid_test.cpp
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#include <unordered_set>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <src/grid.h>

using testing::Each;
using testing::Eq;
using testing::ContainerEq;

namespace sudoku {

// Tests for the Grid class.

// Tests default constructor.
TEST(Grid, constructorDefault) {
  const Grid grid;

  EXPECT_EQ(grid.getSubrows(), 3);
  EXPECT_EQ(grid.getSubcols(), 3);
  for (int i = 0; i < grid.size(); ++i) {
    EXPECT_EQ(grid.getValues(i).count(0), 0);
    for (int j = grid.getMaxValue(); j <= grid.getMaxValue(); ++j)
      EXPECT_EQ(grid.getValues(i).count(j), 1);
  }
}

// Tests constructor with subrow/subcols arguments.
TEST(Grid, constructorTwoArgs) {
  const int subrows = 1, subcols = 2;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getSubrows(), 1);
  EXPECT_EQ(grid.getSubcols(), 2);
  for (int i = 0; i < grid.size(); ++i) {
    EXPECT_EQ(grid.getValues(i).count(0), 0);
    for (int j = grid.getMaxValue(); j <= grid.getMaxValue(); ++j)
      EXPECT_EQ(grid.getValues(i).count(j), 1);
  }
}

// Tests constructor with subrow/subcols/init grid arguments.
TEST(Grid, constructorThreeArgs) {
  const int subrows = 1, subcols = 2;
  const std::vector<int> initGrid = { 1, 2,  0, 0 };
  const Grid grid(subrows, subcols, initGrid);

  EXPECT_EQ(grid.getSubrows(), subrows);
  EXPECT_EQ(grid.getSubcols(), subcols);
  for (int i = 0; i < grid.size(); ++i)
    for (int j = grid.getMaxValue(); j <= grid.getMaxValue(); ++j)
      EXPECT_LE(grid.getValues(i).count(j), 1);
}

// Tests the operator ().
TEST(Grid, operatorParenthesis) {
  const int subrows = 1, subcols = 2;
  Grid grid(subrows, subcols);

  for (int i = 0; i < grid.getNumRows(); i++) {
    for (int j = 0; j < grid.getNumCols(); j++) {
      grid(i, j).emplace(99);
      EXPECT_TRUE(grid(i, j).count(99) == 1);
    }
  }
}

// Tests the operator [].
TEST(Grid, operatorBrackets) {
  const int subrows = 1, subcols = 2;
  Grid grid(subrows, subcols);

  for (int i = 0; i < grid.size(); i++) {
    grid[i].emplace(99);
    EXPECT_TRUE(grid[i].count(99) == 1);
  }
}

// Tests the get rows (in grid) method.
TEST(Grid, getNumRows) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getNumRows(), 6);
}

// Tests the get columns (in grid) method.
TEST(Grid, getNumCols) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getNumCols(), 6);
}

// Tests the get minimum valid value method.
TEST(Grid, getMinValue) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getMinValue(), 1);
}

// Tests the get maximum valid value method.
TEST(Grid, getMaxValue) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getMaxValue(), 6);
}

// Tests the get values vector size method.
TEST(Grid, size) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.size(), 36);
}

// Tests the get value at (row, col) method.
TEST(Grid, getValuesRowCol) {
  const int subrows = 1, subcols = 2;
  Grid grid(subrows, subcols);

  for (int i = 0; i < grid.getNumRows(); i++) {
    for (int j = 0; j < grid.getNumCols(); j++) {
      grid(i, j).emplace(99);
      ASSERT_TRUE(grid(i, j).count(99) == 1);
      EXPECT_TRUE(grid.getValues(i, j).count(99) == 1);
    }
  }
}

// Tests the get value at index method.
TEST(Grid, getValuesIndex) {
  const int subrows = 1, subcols = 2;
  Grid grid(subrows, subcols);

  for (int i = 0; i < grid.size(); i++) {
    grid[i].emplace(99);
    ASSERT_TRUE(grid[i].count(99) == 1);
    EXPECT_TRUE(grid.getValues(i).count(99) == 1);
  }
}

// Tests the get index in grid of (row, col) method.
TEST(Grid, getIndex) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  int index = 0;
  for (int i = 0; i < grid.getNumRows(); i++) {
    for (int j = 0; j < grid.getNumCols(); j++) {
      EXPECT_EQ(grid.getIndex(i, j), index);
      index++;
    }
  }
}

// Tests the get row of index method.
TEST(Grid, getRow) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  int index = 0;
  for (int i = 0; i < grid.getNumRows(); i++) {
    for (int j = 0; j < grid.getNumCols(); j++) {
      EXPECT_EQ(grid.getRow(index), i);
      index++;
    }
  }
}

// Tests the get col of index method.
TEST(Grid, getCol) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  int index = 0;
  for (int i = 0; i < grid.getNumRows(); i++) {
    for (int j = 0; j < grid.getNumCols(); j++) {
      EXPECT_EQ(grid.getCol(index), j);
      index++;
    }
  }
}

// Tests the assign value to (row, col) method.
TEST(Grid, assignValid) {
  const int subrows = 2, subcols = 2;
  const int row = 1, col = 1, val = 3;
  const std::vector<int> initGrid = {
      1, 2,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
  };
  Grid grid(subrows, subcols, initGrid);
  EXPECT_TRUE(grid.assign(row, col, val));
  EXPECT_EQ(grid.getValues(row, col).size(), 1);
  EXPECT_EQ(grid.getValues(row, col).count(val), 1);
  EXPECT_EQ(grid.getValues(row, col - 1).size(), 1);
  EXPECT_EQ(grid.getValues(row + 1, col - 1).size(), 2);
}

TEST(Grid, assignInvalidRow) {
  const int subrows = 2, subcols = 2;
  const int row = 0, col = 2, val = 1;
  const std::vector<int> initGrid = {
      1, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
  };
  Grid grid(subrows, subcols, initGrid);

  EXPECT_FALSE(grid.assign(row, col, val));
  EXPECT_EQ(grid.getValues(row, col).size(), 3);
}

TEST(Grid, assignInvalidCol) {
  const int subrows = 2, subcols = 2;
  int row = 2, col = 0, val = 1;
  const std::vector<int> initGrid = {
      1, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
  };
  Grid grid(subrows, subcols, initGrid);

  EXPECT_FALSE(grid.assign(row, col, val));
  EXPECT_EQ(grid.getValues(row, col).size(), 3);
}

TEST(Grid, assignInvalidSubgrid) {
  const int subrows = 2, subcols = 2;
  const int row = 1, col = 1, val = 1;
  const std::vector<int> initGrid = {
      1, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
  };
  Grid grid(subrows, subcols, initGrid);

  EXPECT_FALSE(grid.assign(row, col, val));
  EXPECT_EQ(grid.getValues(row, col).size(), 3);
}

TEST(Grid, assignInvalidOccupied) {
  const int subrows = 2, subcols = 2;
  const int row = 0, col = 0, val = 1;
  const std::vector<int> initGrid = {
      1, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
      0, 0,  0, 0,
  };
  Grid grid(subrows, subcols, initGrid);

  EXPECT_FALSE(grid.assign(row, col, val));
}

} /* namespace sudoku */
