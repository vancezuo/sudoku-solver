/*
 * solver.cpp
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#include <iterator>
#include <utility>
#include <algorithm>
#include <iostream>

#include <src/solver.h>

using std::vector;

namespace sudoku {

Solver::Solver(Grid grid, VariableSortType varSortType, ValueSortType valSortType):
    grid_(grid) {

  varSortType_ = varSortType;
  valSortType_ = valSortType;
}

bool Solver::solve(Grid& solution, int& steps) {
  steps = 0;
  return solve(solution, steps, grid_);
}

bool Solver::solve(Grid& solution, int& steps, Grid& grid) {
  int index = selectIndex(grid);

  if (index == -1) {
    solution = grid;
    return true;
  }

  vector<int> moves(begin(grid[index]), end(grid[index]));
  sortMoves(grid, moves, index);

  for (auto& value : moves) {
    steps++;

    Grid nextGrid(grid);
    if (nextGrid.assign(index, value) && solve(solution, steps, nextGrid))
      return true;
  }

  return false;
}

int Solver::selectIndex(Grid& grid) {
  switch (varSortType_) {
  case VariableSortType::MOST_CONSTRAINED: {
    int minIndex = -1;
    for (int i = 0; i < grid.size(); ++i) {
      if (grid[i].size() == 1)
        continue;
      if (minIndex == -1 || grid[i].size() < grid[minIndex].size())
        minIndex = i;
    }
    return minIndex;
  }
  case VariableSortType::RANDOMIZED: {
    int numUnset = 0;
    int randIndex = -1;
    for (int i = 0; i < grid.size(); ++i) {
      if (grid[i].size() == 1)
        continue;
      if (std::default_random_engine()() % ++numUnset < 1)
        randIndex = i;
    }
    return randIndex;
  }
  default: {
    for (int i = 0; i < grid.size(); ++i) {
      if (grid[i].size() > 1)
        return i;
    }
    return -1;
  }
  }
}

void Solver::sortMoves(Grid& grid, vector<int>& moves, int index) {
  switch (valSortType_) {
  case ValueSortType::LEAST_CONSTRAINING: {
    vector<int> constrainings(grid.getMaxValue() + 1);
    for (auto& j : moves) {
      for (auto& k : grid.getNeighbors(j)) {
        constrainings[j] += grid[k].count(j);
      }
    }
    std::sort(begin(moves), end(moves), [&](int a, int b) {
      return constrainings[a] < constrainings[b];
    });
  } break;
  case ValueSortType::RANDOMIZED:
    std::shuffle(begin(moves), end(moves), std::default_random_engine());
    break;
  default:
    break;
  }
}


} /* namespace sudoku */
