/*
 * grid_test.cpp
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <src/grid.h>

#include <unordered_set>
#include <vector>

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
  EXPECT_THAT(grid.getValues(), Each(Eq(0)));
}

// Tests constructor with subrow/subcols arguments.
TEST(Grid, constructorTwoArgs) {
  const int subrows = 1, subcols = 2;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getSubrows(), 1);
  EXPECT_EQ(grid.getSubcols(), 2);
  EXPECT_THAT(grid.getValues(), Each(Eq(0)));
}

// Tests constructor with subrow/subcols/init grid arguments.
TEST(Grid, constructorThreeArgs) {
  const int subrows = 1, subcols = 2;
  const std::vector<int> initGrid = { 1, 2,  0, 0 };
  const Grid grid(subrows, subcols, initGrid);

  EXPECT_EQ(grid.getSubrows(), subrows);
  EXPECT_EQ(grid.getSubcols(), subcols);
  EXPECT_EQ(grid.getValues(), initGrid);
}

// Tests the operator ().
TEST(Grid, operatorParenthesis) {
  const int subrows = 1, subcols = 2;
  Grid grid(subrows, subcols);

  int result;
  for (int i = 0; i < grid.getRows(); i++) {
    for (int j = 0; j < grid.getCols(); j++) {
      grid(i, j) = i + j;
      result = grid(i, j);
      EXPECT_EQ(result, i + j);
    }
  }
}

// Tests the get rows (in grid) function.
TEST(Grid, getRows) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getRows(), 6);
}

// Tests the get columns (in grid) function.
TEST(Grid, getCols) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  EXPECT_EQ(grid.getCols(), 6);
}

// Tests the get index in grid of (row, col) function.
TEST(Grid, getIndex) {
  const int subrows = 2, subcols = 3;
  const Grid grid(subrows, subcols);

  int index = 0;
  for (int i = 0; i < grid.getRows(); i++) {
    for (int j = 0; j < grid.getCols(); j++) {
      EXPECT_EQ(grid.getIndex(i, j), index);
      index++;
    }
  }
}

// Tests the get row neighbor values function.
TEST(Grid, getRowValues) {
  const int subrows = 2, subcols = 3;
  const std::vector<int> initGrid = {
      0, 1, 2,  3, 4, 5,
      1, 2, 3,  4, 5, 6,

      2, 3, 4,  5, 6, 7,
      3, 4, 5,  6, 7, 8,

      4, 5, 6,  7, 8, 9,
      5, 6, 7,  8, 9, 10,
  };
  Grid grid(subrows, subcols, initGrid);

  std::unordered_set<int> neighbors({ 0, 1, 2, 3, 4, 5 });
  for (int i = 0; i < grid.getRows(); i++) {
    for (int j = 0; j < grid.getCols(); j++) {
      int ownValue = grid(i, j);
      std::vector<int> v = grid.getRowValues(i, j);
      std::unordered_set<int> result(v.begin(), v.end());
      EXPECT_EQ(result.size(), v.size());

      neighbors.erase(ownValue);
      EXPECT_THAT(result, ContainerEq(neighbors));
      neighbors.insert(ownValue);
    }
    neighbors.erase(i);
    neighbors.insert(i + 6);
  }
}

// Tests the get column neighbor values function.
TEST(Grid, getColValues) {
  const int subrows = 2, subcols = 3;
  const std::vector<int> initGrid = {
      0, 1, 2,  3, 4, 5,
      1, 2, 3,  4, 5, 6,

      2, 3, 4,  5, 6, 7,
      3, 4, 5,  6, 7, 8,

      4, 5, 6,  7, 8, 9,
      5, 6, 7,  8, 9, 10,
  };
  Grid grid(subrows, subcols, initGrid);

  std::unordered_set<int> neighbors({ 0, 1, 2, 3, 4, 5 });
  for (int j = 0; j < grid.getCols(); j++) {
    for (int i = 0; i < grid.getRows(); i++) {
      int ownValue = grid(i, j);
      std::vector<int> v = grid.getColValues(i, j);
      std::unordered_set<int> result(v.begin(), v.end());
      EXPECT_EQ(result.size(), v.size());

      neighbors.erase(ownValue);
      EXPECT_THAT(result, ContainerEq(neighbors));
      neighbors.insert(ownValue);
    }
    neighbors.erase(j);
    neighbors.insert(j + 6);
  }
}

// Tests the get sub-grid neighbor values function.
TEST(Grid, getSubgridValues) {
  const int subrows = 2, subcols = 3;
  const std::vector<int> initGrid = {
      0, 0, 0,  1, 1, 1,
      0, 0, 0,  1, 1, 1,

      2, 2, 2,  3, 3, 3,
      2, 2, 2,  3, 3, 3,

      4, 4, 4,  5, 5, 5,
      4, 4, 4,  5, 5, 5,
  };
  Grid grid(subrows, subcols, initGrid);

  for (int i = 0; i < grid.getRows(); i++) {
    for (int j = 0; j < grid.getCols(); j++) {
      std::vector<int> result = grid.getSubgridValues(i, j);

      EXPECT_EQ(result.size(), 5);
      EXPECT_THAT(result, Each(Eq(grid(i, j))));
    }
  }
}

} /* namespace sudoku */
