/*
 * grid.cpp
 *
 *  Created on: May 12, 2015
 *      Author: Vance Zuo
 */

#include <algorithm>

#include <src/grid.h>

using std::vector;
using std::unordered_set;

namespace sudoku {

Grid::Grid():
    values_(81) {
  subrows_ = 3;
  subcols_ = 3;
  side_ = 9;
  initNeighbors();
  initValues();
}

Grid::Grid(int subrows, int subcols):
    values_((side_ = subrows*subcols) * subrows*subcols) {
  subrows_ = subrows;
  subcols_ = subcols;
  initNeighbors();
  initValues();
}

Grid::Grid(int subrows, int subcols, const vector<int>& grid):
    values_((side_ = subrows*subcols) * subrows*subcols) {
  subrows_ = subrows;
  subcols_ = subcols;
  initNeighbors();
  initValues();
  for (int i = 0; i < (signed) std::min(values_.size(), grid.size()); i++) {
    if (getMinValue() <= grid[i] && grid[i] <= getMaxValue())
      assign(i, grid[i]);
  }
}

void Grid::initValues() {
  for (unsigned int i = 0; i < values_.size(); ++i) {
    values_[i].reserve(getMaxValue() - getMinValue() + 1);
    for (int j = getMinValue(); j <= getMaxValue(); ++j) {
      values_[i].emplace(j);
    }
  }
}

void Grid::initNeighbors() {
  neighbors_ = std::make_shared<vector<vector<int>>>(side_ * side_);
  for (int i = 0; i < size(); ++i) {
    int row = getRow(i);
    int col = getCol(i);

    for (int j = 0; j < side_; ++j) {
      if (col != j)
        (*neighbors_)[i].push_back(getIndex(row, j));
      if (row != j)
        (*neighbors_)[i].push_back(getIndex(j, col));
    }

    const int istart = (row / subrows_) * subrows_;
    const int jstart = (col / subcols_) * subcols_;
    for (int j = istart; j < istart + subrows_; ++j) {
      for (int k = jstart; k < jstart + subcols_; ++k) {
        if (row == j || col == k)
          continue;
        (*neighbors_)[i].push_back(getIndex(j, k));
      }
    }

    std::sort(begin((*neighbors_)[i]), end((*neighbors_)[i]));
  }
}

unordered_set<int>& Grid::operator ()(int row, int col) {
  return values_[getIndex(row, col)];
}

unordered_set<int>& Grid::operator [](int index) {
  return values_[index];
}

int Grid::getNumRows() const {
  return side_;
}

int Grid::getNumCols() const {
  return side_;
}

int Grid::getMinValue() const {
  return 1;
}

int Grid::getMaxValue() const {
  return side_;
}

int Grid::size() const {
  return values_.size();
}

const unordered_set<int>& Grid::getValues(int row, int col) const {
  return values_[getIndex(row, col)];
}

const unordered_set<int>& Grid::getValues(int index) const {
  return values_[index];
}

const std::vector<int>& Grid::getNeighbors(int row, int col) const {
  return (*neighbors_)[getIndex(row, col)];
}

const std::vector<int>& Grid::getNeighbors(int index) const {
  return (*neighbors_)[index];
}

int Grid::getIndex(int row, int col) const {
  return row * getNumCols() + col;
}

int Grid::getRow(int index) const {
  return index / getNumCols();
}

int Grid::getCol(int index) const {
  return index % getNumCols();
}

bool Grid::assign(int row, int col, int value) {
  return assign(getIndex(row, col), value);
}

bool Grid::assign(int index, int value) {
  if (!values_[index].count(value) || values_[index].size() == 1)
    return false;
  values_[index] = { value };
  return propogateFrom(index, value);
}

bool Grid::propogateFrom(int index, int value) {
  for (auto& i : (*neighbors_)[index]) {
    if (!propogateTo(i, value))
      return false;
  }
  return true;
}

bool Grid::propogateTo(int index, int value) {
  if (!values_[index].erase(value))
    return true;
  if (values_[index].empty())
    return false;
  if (values_[index].size() == 1)
    return propogateFrom(index, *values_[index].begin());
  return true;
}

} /* namespace sudoku */
