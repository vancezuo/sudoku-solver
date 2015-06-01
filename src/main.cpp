/*
 * main.cpp
 *
 *  Created on: May 25, 2015
 *      Author: Vance Zuo
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>

#include <src/grid.h>
#include <src/solver.h>

using std::string;
using std::vector;
using std::istream;
using std::ifstream;
using std::locale;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
using sudoku::Grid;
using sudoku::Solver;

void printGrid(const Grid& grid) {
  int numberSpacing = floor(log10(grid.getMaxValue())) + 1;
  for (int i = 0; i < grid.getNumRows(); ++i) {
    for (int j = 0; j < grid.getNumCols(); ++j) {
      if (grid.getValues(i, j).size() == 1) {
        printf("%*d ", numberSpacing, *grid.getValues(i, j).begin());
      } else {
        printf("%*s ", numberSpacing, ".");
      }
    }
    printf("\n");
  }
  fflush(stdout);
}

int readGrid(istream& input, Grid& grid) {
  int value;
  for (int i = 0; i < grid.size(); ) {
    if (input.peek() == EOF)
      return i;
    if (!(input >> value)) {
      input.clear();
      input.ignore(1);
      continue;
    }
    grid.assign(i++, value);
  }
  return grid.size();
}

void printUsage() {
  printf("Usage: sudoku-solver.exe [-d SUBROWS SUBCOLS] [FILE]\n");
}

// Main method
// Args: [-d SUBROWS SUBCOLS] [FILE]
int main(int argc, char **argv) {
  int subrows = 3;
  int subcols = 3;
  istream *input;
  ifstream ifs;

  int i = 1;
  if (i < argc && !strcmp(argv[i], "-d")) {
    i = 4;
    if (*argv[2] == '\0' || *argv[3] == '\0') {
      printf("Error: Not enough arguments after -d.\n");
      printUsage();
      exit(1);
    }
    subrows = atoi(argv[2]);
    subcols = atoi(argv[3]);
    if (subrows == 0 || subcols == 0) {
      printf("Error: Illegal subrow/col arguments (should be integers >0).\n");
      printUsage();
      exit(1);
    }
  }

  if (i < argc && *argv[i] != '\0') {
    ifs.open(argv[i]);
    if (!ifs.is_open()) {
      printf("Error: Could not open file.\n");
      printUsage();
      exit(1);
    }
    input = &ifs;
  } else {
    input = &std::cin;
    printf("Enter the grid as a list of values (0 for blank):\n");
  }

  int completedGrids = 0;
  double totalNs = 0;
  while (true) {
    Grid grid(subrows, subcols);
    int numValues = readGrid(*input, grid);
    if (numValues < grid.size()) {
      if (numValues > 0 || completedGrids == 0)
        printf("Warning: Incomplete definition (%d values).\n", numValues);
      break;
    }
    if (input != &std::cin)
      printGrid(grid);

    auto begin = high_resolution_clock::now();

    Solver solver(grid);
    Grid solution;
    int steps;
    bool solved = solver.solve(solution, steps);

    auto end = high_resolution_clock::now();
    auto ns = duration_cast<nanoseconds>(end - begin).count();
    totalNs += ns;

    printf(" ||\n");
    printf(" || (%d steps, %.3fs)\n", steps, ns / 1000000000.0);
    printf(" \\/\n");
    if (solved) {
      printGrid(solution);
    } else {
      printf("[NO SOLUTION]\n");
    }
    printf("----\n");

    completedGrids++;
  }
  printf("Solved %d grids (%.3fs)", completedGrids, totalNs / 1000000000.0);

  if (ifs.is_open())
    ifs.close();

  exit(0);
}



