/*
 * grid.h
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#ifndef GRID_H_
#define GRID_H_

#include <vector>

namespace sudoku {

class Grid {
public:
  Grid();
  Grid(int subrows, int subcols);
  Grid(int subrows, int subcols, std::vector<int> grid);

  int& operator ()(int row, int col);
  int& operator [](int index);

  int getSubrows() const { return subrows_; }
  int getSubcols() const { return subcols_; }
  const std::vector<int>& getValues() const { return values_; }

  int getRows() const;
  int getCols() const;
  int getMinValue() const;
  int getMaxValue() const;
  int getSize() const;

  int getValue(int row, int col) const;
  int getValue(int index) const;

  int getIndex(int row, int col) const;
  int getRow(int index) const;
  int getCol(int index) const;

  std::vector<int> getRowValues(int row, int col) const;
  std::vector<int> getColValues(int row, int col) const;
  std::vector<int> getSubgridValues(int row, int col) const;

private:
  int subrows_;
  int subcols_;
  int side_;
  std::vector<int> values_;
};

} /* namespace sudoku */

#endif /* GRID_H_ */
