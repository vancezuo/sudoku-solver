/*
 * solver.h
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <vector>
#include <unordered_set>

#include <src/grid.h>

namespace sudoku {

enum class VariableSortType {
  NONE, RANDOMIZED, MOST_CONSTRAINED
};

enum class ValueSortType {
  NONE, RANDOMIZED, LEAST_CONSTRAINING
};

class Solver {
public:
  Solver(Grid grid,
      VariableSortType varSortType = VariableSortType::MOST_CONSTRAINED,
      ValueSortType valSortType = ValueSortType::LEAST_CONSTRAINING);

  const Grid& getGrid() const { return grid_; }
  const std::vector<bool>& getOrigSetVec() const {
    return origSet_;
  }
  const std::unordered_set<int>& getUnsetIndexes() const {
    return unsetIndexes_;
  }
  const std::vector<std::unordered_set<int>>& getMovesVec() const {
    return movesVec_;
  }
  const std::vector<std::vector<int>>& getHistoryVec() const {
    return historyVec_;
  }

  bool isOrigSet(int row, int col) const;
  const std::unordered_set<int>& getMoves(int row, int col) const;
  const std::vector<int>& getHistory(int row, int col) const;

  bool assign(int row, int col, int value);
  bool unassign(int row, int col);

  int solve();
  void reset();

private:
  Grid grid_;
  std::vector<bool> origSet_;
  std::unordered_set<int> unsetIndexes_;
  std::vector<std::unordered_set<int>> movesVec_;
  std::vector<std::vector<int>> historyVec_;

  VariableSortType varSortType_;
  ValueSortType valSortType_;

  bool propogateConstraint(int row, int col, int value, int origIndex);
  bool unpropogateConstraint(int row, int col, int value);
  void sortIndexes(std::vector<int>& indexes);
  void sortMoves(std::vector<int>& moves, int index);
};

} /* namespace sudoku */

#endif /* SOLVER_H_ */
