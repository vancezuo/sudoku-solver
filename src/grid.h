/*
 * grid.h
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#ifndef GRID_H_
#define GRID_H_

#include <unordered_set>
#include <vector>

namespace sudoku {

class Grid {
public:
  Grid();
  Grid(int subrows, int subcols);
  Grid(int subrows, int subcols, const std::vector<int>& grid);

  std::unordered_set<int>& operator ()(int row, int col);
  std::unordered_set<int>& operator [](int index);

  int getSubcols() const { return subcols_; }
  int getSubrows() const { return subrows_; }

  int getNumRows() const;
  int getNumCols() const;
  int getMinValue() const;
  int getMaxValue() const;
  int size() const;

  const std::vector<std::unordered_set<int>>& getValues() const { return values_; }
  const std::unordered_set<int>& getValues(int row, int col) const;
  const std::unordered_set<int>& getValues(int index) const;

  const std::vector<std::vector<int>>& getNeighbors() const { return neighbors_; }
  const std::vector<int>& getNeighbors(int row, int col) const;
  const std::vector<int>& getNeighbors(int index) const;

  int getIndex(int row, int col) const;
  int getRow(int index) const;
  int getCol(int index) const;

  bool assign(int row, int col, int value);
  bool assign(int index, int value);

private:
  int subrows_, subcols_;
  int side_;
  std::vector<std::unordered_set<int>> values_;
  std::vector<std::vector<int>> neighbors_;

  void initNeighbors();
  void initValues();
  bool propogateFrom(int index, int value);
  bool propogateTo(int index, int value);
};

} /* namespace sudoku */

#endif /* GRID_H_ */
