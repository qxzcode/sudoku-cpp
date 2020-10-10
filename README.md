# SudokuGen

This is a simple, self-contained C++ program that generates valid solved sudoku puzzles. It uses a recursive backtracking algorithm to fill in digits while adhering to the restriction of not having any duplicate digits in any of the 9 rows, columns, or 3x3 squares.

## Building

I recommend using the included CMake configuration to build this. However, it's just a single C++ source file that only depends on the standard library, so if you're having trouble with CMake you can probably just directly invoke your compiler on `src/main.cpp`.

Here's how to build it with CMake+Make from a UNIX-based terminal, starting in the root repository directory:

```
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make
```

The compiled binary is called `SudokuGen` in the created `build/` directory.

## Usage

The program takes no arguments and no input. It simply generates a random solved sudoku grid and spits it out:

```
$ ./SudokuGen
972651384
651483792
348297651
826519437
794836125
135742968
567924813
483165279
219378546
```
