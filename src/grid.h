/*
 * grid.h
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#ifndef GRID_H_
#define GRID_H_

#include <vector>

using std::vector;

namespace sudoku {

class Grid {
public:
  Grid();
  Grid(int subrows, int subcols);
  Grid(int subrows, int subcols, vector<int> grid);

  int& operator ()(int row, int col);

  int getSubrows() const { return subrows_; }
  int getSubcols() const { return subcols_; }
  const vector<int>& getValues() const { return values_; }

  int getRows() const;
  int getCols() const;
  int getRange() const;
  int getSize() const;

  int getValue(int row, int col) const;
  int getIndex(int row, int col) const;

  vector<int> getRowValues(int row, int col) const;
  vector<int> getColValues(int row, int col) const;
  vector<int> getSubgridValues(int row, int col) const;

private:
  int subrows_;
  int subcols_;
  int side_;
  vector<int> values_;
};

} /* namespace sudoku */

#endif /* GRID_H_ */
