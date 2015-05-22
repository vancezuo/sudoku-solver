/*
 * solver.h
 *
 *  Created on: May 14, 2015
 *      Author: Vance Zuo
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <src/grid.h>

#include <vector>
#include <unordered_set>

using std::vector;
using std::pair;
using std::unordered_set;

namespace sudoku {

namespace VariableSortType { enum VarSort {
  NONE, RANDOMIZED, MOST_CONSTRAINED
}; }

namespace ValueSortType { enum ValSort {
  NONE // , RANDOMIZED, LEAST_CONSTRAINING
}; }

class Solver {
public:

  Solver(Grid grid,
      VariableSortType::VarSort varSort=VariableSortType::MOST_CONSTRAINED,
      ValueSortType::ValSort valSort=ValueSortType::NONE);

  const Grid& getGrid() const { return grid_; }
  const vector<bool>& getOrigSetVec() const { return origSet_; }
  const unordered_set<int>& getUnsetIndexes() const { return unsetIndexes_; }
  const vector<unordered_set<int>>& getMovesVec() const { return movesVec_; }
  const vector<vector<int>>& getHistoryVec() const { return historyVec_; }

  bool isOrigSet(int row, int col) const;
  const unordered_set<int>& getMoves(int row, int col) const;
  const vector<int>& getHistory(int row, int col) const;

  bool assign(int row, int col, int value);
  bool unassign(int row, int col);

  int solve();
  void reset();


private:
  Grid grid_;
  vector<bool> origSet_;
  unordered_set<int> unsetIndexes_;
  vector<unordered_set<int>> movesVec_;
  vector<vector<int>> historyVec_;

  VariableSortType::VarSort varSort_;
  ValueSortType::ValSort valSort_;

  bool propogateConstraint(int row, int col, int value, int origIndex);
  bool unpropogateConstraint(int row, int col, int value);

};

} /* namespace sudoku */

#endif /* SOLVER_H_ */
