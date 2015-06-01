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
  ValueSortType getValSortType() const { return valSortType_; }
  VariableSortType getVarSortType() const { return varSortType_; }

  bool solve(Grid& solution, int& steps);
  // bool solveAll(std::vector<Grid>& solutions, std::vector<int>& steps);

private:
  Grid grid_;

  VariableSortType varSortType_;
  ValueSortType valSortType_;

  bool solve(Grid& solution, int& steps, Grid& grid);
  int selectIndex(Grid& grid);
  void sortMoves(Grid& grid, std::vector<int>& moves, int index);
};

} /* namespace sudoku */

#endif /* SOLVER_H_ */
