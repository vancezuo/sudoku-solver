/*
 * solver.cpp
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#include <src/solver.h>
#include <iterator>
#include <utility>

namespace sudoku {

Solver::Solver(Grid grid):
        grid_(grid.getSubrows(), grid.getSubcols()),
        origSet_(grid.getSize()),
        movesVec_(grid.getSize()),
        historyVec_(grid.getSize()) {

  // Enables all moves for each square.
  for (int i = 0; i < grid.getSize(); ++i) {
    for (int j = 1; j <= grid.getRange(); ++j)
      movesVec_[i].emplace(j);
  }

  // Assigns squares with preset values.
  for (int i = 0; i < grid.getSize(); ++i) {
    if (grid[i] != 0) {
      origSet_[i] = assign(i / grid.getCols(), i % grid.getCols(), grid[i]);
    } else {
      unsetIndexes_.emplace(i);
    }
  }
}

bool Solver::isOrigSet(int row, int col) const {
  return origSet_[grid_.getIndex(row, col)];
}

const unordered_set<int>& sudoku::Solver::getMoves(int row, int col) const {
  return movesVec_[grid_.getIndex(row, col)];
}

const vector<int>& sudoku::Solver::getHistory(int row, int col) const {
  return historyVec_[grid_.getIndex(row, col)];
}

bool Solver::assign(int row, int col, int value) {
  int origIndex = grid_.getIndex(row, col);

  if (movesVec_[origIndex].erase(value) <= 0)
    return false;
  grid_(row, col) = value;
  unsetIndexes_.erase(origIndex);
  historyVec_[origIndex].push_back(origIndex);

  // Check for row or column contradictions.
  for (int i = 0; i < grid_.getCols(); i++) {
    if (i != col && !propogateConstraint(row, i, value, origIndex)) {
      unassign(row, col);
      return false;
    }
    if (i != row && !propogateConstraint(i, col, value, origIndex)) {
      unassign(row, col);
      return false;
    }
  }

  // Check for subgrid contradictions.
  const int istart = (row / grid_.getSubrows()) * grid_.getSubrows();
  const int jstart = (col / grid_.getSubcols()) * grid_.getSubcols();
  for (int i = istart; i < istart + grid_.getSubrows(); i++) {
    for (int j = jstart; j < jstart + grid_.getSubcols(); j++) {
      if (row == i || col == j)
        continue;
      if (!propogateConstraint(i, j, value, origIndex)) {
        unassign(row, col);
        return false;
      }
    }
  }

  return true;
}

bool Solver::propogateConstraint(int row, int col, int value, int origIndex) {
  if (grid_(row, col) == value)
    return false;

  int index = grid_.getIndex(row, col);
  movesVec_[index].erase(value);
  historyVec_[origIndex].push_back(index);

  if (grid_(row, col) <= 0 && movesVec_[index].empty())
    return false;

  return true;
}

bool Solver::unassign(int row, int col) {
  if (origSet_[grid_.getIndex(row, col)])
    return false;
  int value = grid_(row, col);
  if (value <= 0)
    return false;
  grid_(row, col) = 0;
  unsetIndexes_.emplace(grid_.getIndex(row, col));
  return unpropogateConstraint(row, col, value);
}

bool Solver::unpropogateConstraint(int row, int col, int value) {
  int index = grid_.getIndex(row, col);
  if (historyVec_[index].empty())
    return false;
  while (!historyVec_[index].empty()) {
    movesVec_[historyVec_[index].back()].emplace(value);
    historyVec_[index].pop_back();
  }
  return true;
}

int Solver::solve() {
  int steps = 0;
  for (int i = 0; i < grid_.getSize(); ++i) {
    if (unsetIndexes_.count(i) == 0)
      continue;
    for (int j = 1; j <= grid_.getRange(); ++j) {
      if (movesVec_[i].count(j) == 0)
        continue;
      int row = i / grid_.getCols();
      int col = i % grid_.getCols();

      steps++;

      if (!assign(row, col, j))
        continue;

      steps += solve();

      if (unsetIndexes_.empty())
        return steps;

      unassign(row, col);
    }
  }

  return steps;
}

void Solver::solveHelper() {
}

void Solver::reset() {
  for (int i = 0; i < grid_.getRows(); ++i) {
    for (int j = 0; j < grid_.getCols(); ++j) {
      if (origSet_[grid_.getIndex(i, j)])
        continue;
      unassign(i, j);
    }
  }
}


} /* namespace sudoku */

