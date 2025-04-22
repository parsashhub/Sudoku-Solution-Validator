# Sudoku Solution Validator

This project is a multithreaded C program that validates a Sudoku puzzle solution using the pthread library. The program checks if a given 9x9 Sudoku grid is valid by ensuring that each row, column, and 3x3 subgrid contains the numbers 1 to 9 without repetition.

## Implementation Details

The program uses multiple threads to validate the Sudoku grid:

- A separate thread is created for each row, column, and subgrid.
- The program checks the validity of the Sudoku solution by verifying each row, column, and subgrid concurrently.

## Files

- `sudoku_validator.c`: The main C source file containing the implementation of the Sudoku validator.

## Building and Running the Program

To compile and run the program, follow these steps:

1. Open a terminal and navigate to the directory containing the `sudoku_validator.c` file.

2. Compile the program using `gcc`:

   ```bash
   gcc -o sudoku_validator sudoku_validator.c -lpthread
   ```

3. Run the compiled executable:

   ```bash
   ./sudoku_validator
   ```

The program will output whether the Sudoku solution is valid or invalid.

## Dependencies

- GCC compiler
- pthread library

Ensure that you have the necessary permissions to compile and run C programs on your system.