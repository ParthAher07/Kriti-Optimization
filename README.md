## ABOUT THE PROJECT:

- The objective of this project is to construct a polygonal containment field that captures the maximum value of crystals on a 2D grid while minimizing the inclusion of void mines.

- This is an NP-Hard optimization problem, and our approach uses efficient heuristics such as:
  - Kadane’s Algorithm (extended to 2D for rectangular subregions).
  - Dynamic Programming strategies for optimal substructure exploitation.
  - Our solution aims to provide a near-optimal answer in a reasonable time, balancing performance with accuracy.



# Optimization(How to run?)

## Prerequisites

- C++ compiler (g++ recommended)
- Make sure you have C++11 or higher


## Compilation

To compile the program, use the following command:

```bash
g++ -o program 57_optimization_code.cpp
```

If you need to specify a specific C++ standard, use:
```bash
g++ -std=c++11 -o program 57_optimization_code.cpp
```

## Running the Program

After compilation, run the program using:
```bash
./program
```
You will have to enter the number of test cases in the terminal when asked to.

## Output

The program will:
1. Read data from all input files
2. Process the grid data
3. Generate output in separate output files
4. Display the results in the console

## Input File Format
Input files must be named as "input00.txt", "input01.txt" and so on.

Each input file should contain:
- First line: Number N (integer)
- Next N lines: Three space-separated integers (x, y, value)
- Then: Number M (integer)
- Next M lines: Three space-separated integers (x, y, value)
Make sure that the input files are in the same directory as the code file

## Troubleshooting

If you encounter errors:
1. Check if the input file exists and is correctly formatted
2. Ensure you have write permissions in the directory (for output files)
3. Make sure you are using a compatible C++ version

## Memory Requirements

This program uses large vectors (10000 x 10000). Therefore, we recommend you to ensure your system has sufficient memory available.
