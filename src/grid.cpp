/*
 * grid.cpp
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#include <vector>
#include <algorithm>

#include <src/grid.h>

using std::vector;

namespace sudoku {

Grid::Grid():
    values_(81) {
  subrows_ = 3;
  subcols_ = 3;
  side_ = 9;
}

Grid::Grid(int subrows, int subcols):
    values_((side_ = subrows*subcols) * subrows*subcols) {
  subrows_ = subrows;
  subcols_ = subcols;
}

Grid::Grid(int subrows, int subcols, vector<int> grid):
    values_((side_ = subrows*subcols) * subrows*subcols) {
  subrows_ = subrows;
  subcols_ = subcols;
  for (size_t i = 0; i < std::min(values_.size(), grid.size()); i++)
    values_[i] = grid[i];
}

int& Grid::operator ()(int row, int col) {
  return values_[getIndex(row, col)];
}

int& Grid::operator [](int index) {
  return values_[index];
}

int Grid::getRows() const {
  return side_;
}

int Grid::getCols() const {
  return side_;
}

int Grid::getMinValue() const {
  return 1;
}

int Grid::getMaxValue() const {
  return side_;
}

int Grid::getSize() const {
  return values_.size();
}

int Grid::getValue(int row, int col) const {
  return values_[getIndex(row, col)];
}

int Grid::getValue(int index) const {
  return values_[index];
}

int Grid::getIndex(int row, int col) const {
  return row * getCols() + col;
}


int Grid::getRow(int index) const {
  return index / getCols();
}

int Grid::getCol(int index) const {
  return index % getCols();
}

vector<int> Grid::getRowValues(int row, int col) const {
  vector<int> values;
  values.reserve(getCols() - 1);
  for (int i = 0; i < getCols(); i++) {
    if (i == col)
      continue;
    values.push_back(values_[getIndex(row, i)]);
  }
  return values;
}

vector<int> Grid::getColValues(int row, int col) const {
  vector<int> values;
  values.reserve(getRows() - 1);
  for (int i = 0; i < getRows(); i++) {
    if (i == row)
      continue;
    values.push_back(values_[getIndex(i, col)]);
  }
  return values;
}

vector<int> Grid::getSubgridValues(int row, int col) const {
  vector<int> values;
  values.reserve(getSubrows()*getSubcols() - 1);
  const int istart = (row / getSubrows()) * getSubrows();
  const int jstart = (col / getSubcols()) * getSubcols();
  for (int i = istart; i < istart + getSubrows(); i++) {
    for (int j = jstart; j < jstart + getSubcols(); j++) {
      if (row == i && col == j)
        continue;
      values.push_back(values_[getIndex(i, j)]);
    }
  }
  return values;
}

} /* namespace sudoku */
