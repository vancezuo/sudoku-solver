# sudoku-solver

This is a simple solver for generalized Sudoku. In its strongest mode it uses a backtracking algorithm with the most constrained heuristic for variable (location) ordering and the least constraining heuristic for value ordering. On 'typical' 3x3 Sudoku puzzles it reaches a solution in 10s to 100s of milliseconds.

## Requirements
- C++11

## Setup
All the source files are in the `src` directory, and require an include path at the project root directory. Compile them using your preferred method. For example using g++ go to `src` and run

```sh
g++ -std=c++11 -I.. -o sudoku main.cpp solver.cpp grid.cpp
```

creating a `sudoku` console program. For fastest performance add the `-O3` optimization option.

## Usage
Where `sudoku` is the executable's name, the programs syntax follows

```sh
sudoku [-d SUB_ROWS SUB_COLS] [INPUT_FILE]
```

The `-d` allows specifying custom dimensions for a Sudoku subgrid (default: 3x3). The `INPUT_FILE` is a file that contains grid definitions to solve. A grid definition is a list of of the grid's values, with 0 representing an empty square. The values can be delimited by any non-numeric characters. For example, the following three 3x3 grids are valid:

```
0 9 4 0 0 0 1 3 0 
0 0 0 0 0 0 0 0 0 
0 0 0 0 7 6 0 0 2 
0 8 0 0 1 0 0 0 0 
0 3 2 0 0 0 0 0 0 
0 0 0 2 0 0 0 6 0 
0 0 0 0 5 0 4 0 0 
0 0 0 0 0 8 0 0 7 
0 0 6 3 0 4 0 0 8 

+-----+-----+-----+
|0 0 0|0 0 0|0 0 0|
|0 0 0|0 0 3|0 8 5|
|0 0 1|0 2 0|0 0 0|
+-----+-----+-----+
|0 0 0|5 0 7|0 0 0|
|0 0 4|0 0 0|1 0 0|
|0 9 0|0 0 0|0 0 0|
+-----+-----+-----+
|5 0 0|0 0 0|0 7 3|
|0 0 2|0 1 0|0 0 0|
|0 0 0|0 4 0|0 0 9|
+-----+-----+-----+

1.0.0.0.0.7.0.9.0.0.3.0.0.2.0.0.0.8.0.0.9.6.0.0.5.0.0
0.0.5.3.0.0.9.0.0.0.1.0.0.8
0.0.0.2.6.0.0.0.0.4.0.0.0.3
0.0.0.0.0.0.1
0.0.4.0.0.0.0
0.0.7.0
0.7.0.0
0.3.0.0 
```

If the `INPUT_FILE` is unspecified then the program reads from `stdin`. When the algorithm has found a solution it outputs the solved grid and the time taken (as well as 'steps' in the algorithm -- number of search nodes traversed). When it has read all the grids in the input it also outputs the total time used for all puzzles. For the three grids above for example, the output would be something like

```
. 9 4 . . . 1 3 .
. . . . . . . . .
. . . . 7 6 . . 2
. 8 . . 1 . . . .
. 3 2 . . . . . .
. . . 2 . . . 6 .
. . . . 5 . 4 . .
. . . . . 8 . . 7
. . 6 3 . 4 . . 8
 ||
 || (1108 steps, 0.042s)
 \/
7 9 4 5 8 2 1 3 6
2 6 8 9 3 1 7 4 5
3 1 5 4 7 6 9 8 2
6 8 9 7 1 5 3 2 4
4 3 2 8 6 9 5 7 1
1 5 7 2 4 3 8 6 9
8 2 1 6 5 7 4 9 3
9 4 3 1 2 8 6 5 7
5 7 6 3 9 4 2 1 8
----
. . . . . . . . .
. . . . . 3 . 8 5
. . 1 . 2 . . . .
. . . 5 . 7 . . .
. . 4 . . . 1 . .
. 9 . . . . . . .
5 . . . . . . 7 3
. . 2 . 1 . . . .
. . . . 4 . . . 9
 ||
 || (18115 steps, 0.573s)
 \/
9 8 7 6 5 4 3 2 1
2 4 6 1 7 3 9 8 5
3 5 1 9 2 8 7 4 6
1 2 8 5 3 7 6 9 4
6 3 4 8 9 2 1 5 7
7 9 5 4 6 1 8 3 2
5 1 9 2 8 6 4 7 3
4 7 2 3 1 9 5 6 8
8 6 3 7 4 5 2 1 9
----
1 . . . . 7 . 9 .
. 3 . . 2 . . . 8
. . 9 6 . . 5 . .
. . 5 3 . . 9 . .
. 1 . . 8 . . . 2
6 . . . . 4 . . .
3 . . . . . . 1 .
. 4 . . . . . . 7
. . 7 . . . 3 . .
 ||
 || (196 steps, 0.008s)
 \/
1 6 2 8 5 7 4 9 3
5 3 4 1 2 9 6 7 8
7 8 9 6 4 3 5 2 1
4 7 5 3 1 2 9 8 6
9 1 3 5 8 6 7 4 2
6 2 8 7 9 4 1 3 5
3 5 6 4 7 8 2 1 9
2 4 1 9 3 5 8 6 7
8 9 7 2 6 1 3 5 4
----
Solved 3 grids (0.623s)
```
