/*
 * solver.cpp
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#include <iterator>
#include <utility>
#include <algorithm>

#include <src/solver.h>

using std::vector;
using std::unordered_set;

namespace sudoku {

Solver::Solver(Grid grid, VariableSortType varSortType,
    ValueSortType valSortType):
    grid_(grid.getSubrows(), grid.getSubcols()),
    origSet_(grid.getSize()),
    movesVec_(grid.getSize()),
    historyVec_(grid.getSize()) {

  varSortType_ = varSortType;
  valSortType_ = valSortType;

  // Enables all moves for each square.
  for (int i = 0; i < grid.getSize(); ++i) {
    for (int j = 1; j <= grid.getMaxValue(); ++j)
      movesVec_[i].emplace(j);
  }

  // Assigns squares with preset values.
  for (int i = 0; i < grid.getSize(); ++i) {
    if (grid[i] != 0) {
      origSet_[i] = assign(grid.getRow(i), grid.getCol(i), grid[i]);
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

void Solver::sortIndexes(vector<int>& indexes) {
  switch (varSortType_) {
  case VariableSortType::MOST_CONSTRAINED:
    std::sort(begin(indexes), end(indexes), [&](int a, int b) {
      return movesVec_[a].size() < movesVec_[b].size();
    });
    break;
  case VariableSortType::RANDOMIZED:
    std::random_shuffle(begin(indexes), end(indexes));
    break;
  default:
    break;
  }
}

void Solver::sortMoves(vector<int>& moves, int index) {
  switch (valSortType_) {
  case ValueSortType::LEAST_CONSTRAINING: {
    vector<int> constrain(grid_.getMaxValue() + 1);
    int row = grid_.getRow(index);
    int col = grid_.getCol(index);
    for (auto& j : moves) {
      for (int k = 0; k < grid_.getCols(); k++) {
        if (k != col)
          constrain[j] += movesVec_[grid_.getIndex(row, k)].count(j);
        if (k != row)
          constrain[j] += movesVec_[grid_.getIndex(k, col)].count(j);
      }
      int istart = (row / grid_.getSubrows()) * grid_.getSubrows();
      int jstart = (col / grid_.getSubcols()) * grid_.getSubcols();
      for (int k = istart; k < istart + grid_.getSubrows(); k++) {
        for (int l = jstart; l < jstart + grid_.getSubcols(); l++) {
          if (row == k || col == l)
            continue;

          constrain[j] += movesVec_[grid_.getIndex(k, l)].count(j);
        }
      }
    }
    std::sort(begin(moves), end(moves), [&](int a, int b) {
      return constrain[a] < constrain[b];
    });
  } break;
  case ValueSortType::RANDOMIZED:
    std::random_shuffle(begin(moves), end(moves));
    break;
  default:
    break;
  }
}

int Solver::solve() {
  int steps = 0;

  vector<int> unsetIndexes(begin(unsetIndexes_), end(unsetIndexes_));
  sortIndexes(unsetIndexes);
  for (auto& i : unsetIndexes) {
    vector<int> moves(begin(movesVec_[i]), end(movesVec_[i]));
    sortMoves(moves, i);
    for (auto& j : moves) {
      int row = grid_.getRow(i);
      int col = grid_.getCol(i);

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

