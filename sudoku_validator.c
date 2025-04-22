#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @file sudoku_validator.c
 * @brief A multi-threaded program to validate Sudoku solutions
 *
 * This program uses POSIX threads to concurrently validate a 9x9 Sudoku solution.
 * It creates separate threads to check each row, column, and 3x3 subgrid.
 */

#define SIZE 9  // Size of the Sudoku grid (9x9)

/**
 * @struct parameters
 * @brief Structure to pass parameters to validation threads
 *
 * This structure contains all necessary data for a thread to validate
 * a specific row, column, or subgrid of the Sudoku puzzle.
 */
typedef struct {
    int (*sudoku)[SIZE];  // Pointer to the Sudoku grid
    int index;            // Index of row, column, or subgrid to check
    int *valid;           // Shared array to store validation results
} parameters;

/**
 * @brief Thread function to validate all rows of the Sudoku grid
 *
 * Checks if all rows contain numbers 1-9 exactly once.
 * Sets the corresponding element in the valid array to 0 if invalid, 1 if valid.
 *
 * @param param Pointer to parameters structure containing grid data
 * @return NULL
 */
void* check_rows(void* param) {
    parameters* p = (parameters*)param;
    int* valid = p->valid;
    
    // Check each row
    for (int row = 0; row < SIZE; row++) {
        bool seen[SIZE + 1] = {false};       // Array to track seen numbers (1-9)
        
        // Check each cell in the row
        for (int col = 0; col < SIZE; col++) {
            int num = p->sudoku[row][col];   // Get the number at current position
            
            // Check if number is invalid (out of range 1-9 or already seen in this row)
            if (num < 1 || num > 9 || seen[num]) {
                valid[0] = 0;               // Mark rows as invalid (index 0 in results array)
                pthread_exit(NULL);          // Exit thread early
            }
            seen[num] = true;               // Mark this number as seen
        }
    }
    
    valid[0] = 1;                           // Mark rows as valid
    pthread_exit(NULL);                     // Exit thread
}

/**
 * @brief Thread function to validate all columns of the Sudoku grid
 *
 * Checks if all columns contain numbers 1-9 exactly once.
 * Sets the corresponding element in the valid array to 0 if invalid, 1 if valid.
 *
 * @param param Pointer to parameters structure containing grid data
 * @return NULL
 */
void* check_cols(void* param) {
    parameters* p = (parameters*)param;
    int* valid = p->valid;
    
    // Check each column
    for (int col = 0; col < SIZE; col++) {
        bool seen[SIZE + 1] = {false};       // Array to track seen numbers (1-9)
        
        // Check each cell in the column
        for (int row = 0; row < SIZE; row++) {
            int num = p->sudoku[row][col];   // Get the number at current position
            
            // Check if number is invalid (out of range 1-9 or already seen in this column)
            if (num < 1 || num > 9 || seen[num]) {
                valid[1] = 0;               // Mark columns as invalid (index 1 in results array)
                pthread_exit(NULL);          // Exit thread early
            }
            seen[num] = true;               // Mark this number as seen
        }
    }
    
    valid[1] = 1;                           // Mark columns as valid
    pthread_exit(NULL);                     // Exit thread
}

/**
 * @brief Thread function to validate a single 3x3 subgrid of the Sudoku grid
 *
 * Checks if a specific 3x3 subgrid contains all numbers 1-9 exactly once.
 * Sets the corresponding element in the valid array to 0 if invalid, 1 if valid.
 *
 * @param param Pointer to parameters structure containing subgrid index and grid data
 * @return NULL
 */
void* check_subgrid(void* param) {
    parameters* p = (parameters*)param;
    int grid = p->index;                 // Get the subgrid index (0-8)
    
    // Calculate the starting row and column of this 3x3 subgrid
    // Grid indices are arranged as:
    // 0 1 2
    // 3 4 5
    // 6 7 8
    int start_row = (grid / 3) * 3;      // Integer division gives row offset (0, 3, or 6)
    int start_col = (grid % 3) * 3;      // Modulo gives column offset (0, 3, or 6)
    
    bool seen[SIZE + 1] = {false};       // Array to track seen numbers (1-9)
    
    // Check each cell in the 3x3 subgrid
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = p->sudoku[start_row + i][start_col + j];  // Get number at current position
            
            // Check if number is invalid (out of range 1-9 or already seen in this subgrid)
            if (num < 1 || num > 9 || seen[num]) {
                p->valid[grid + 2] = 0;  // Mark subgrid as invalid (indices 2-10 in results array)
                pthread_exit(NULL);       // Exit thread early
            }
            seen[num] = true;            // Mark this number as seen
        }
    }
    
    p->valid[grid + 2] = 1;              // Mark subgrid as valid
    pthread_exit(NULL);                  // Exit thread
}

/**
 * @brief Example of a valid Sudoku solution
 * 
 * This grid contains all numbers 1-9 exactly once in each row, column, and 3x3 subgrid.
 */
int valid_sudoku[SIZE][SIZE] = {
    {5,3,4,6,7,8,9,1,2},
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

/**
 * @brief Example of an invalid Sudoku solution
 * 
 * This grid has duplicate numbers in the first row (two 5s),
 * which makes it invalid according to Sudoku rules.
 */
int invalid_sudoku[SIZE][SIZE] = {
    {5,3,4,6,7,8,9,1,5},  // Invalid: contains two 5s in the first row
    {6,7,2,1,9,5,3,4,8},
    {1,9,8,3,4,2,5,6,7},
    {8,5,9,7,6,1,4,2,3},
    {4,2,6,8,5,3,7,9,1},
    {7,1,3,9,2,4,8,5,6},
    {9,6,1,5,3,7,2,8,4},
    {2,8,7,4,1,9,6,3,5},
    {3,4,5,2,8,6,1,7,9}
};

/**
 * @brief Function to validate a Sudoku solution using multiple threads
 *
 * @param sudoku_grid Pointer to the 9x9 Sudoku grid to validate
 * @return true if the Sudoku solution is valid, false otherwise
 */
bool validate_sudoku(int (*sudoku_grid)[SIZE]) {
    pthread_t threads[11];              // Array to hold thread IDs (1 for rows + 1 for columns + 9 for subgrids)
    parameters params[11];              // Array to hold parameters for each thread
    int valid[11] = {0};               // Array to store validation results

    // Create thread for all rows (index 0)
    params[0].sudoku = sudoku_grid;     // Pass the Sudoku grid to check
    params[0].index = 0;                // Not used in the new implementation
    params[0].valid = valid;           // Shared results array
    pthread_create(&threads[0], NULL, check_rows, &params[0]);
    
    // Create thread for all columns (index 1)
    params[1].sudoku = sudoku_grid;
    params[1].index = 0;                // Not used in the new implementation
    params[1].valid = valid;
    pthread_create(&threads[1], NULL, check_cols, &params[1]);
    
    // Create threads for 3x3 subgrids (indices 2-10)
    for (int i = 0; i < SIZE; i++) {
        params[i + 2].sudoku = sudoku_grid;
        params[i + 2].index = i;        // Subgrid index
        params[i + 2].valid = valid;
        pthread_create(&threads[i + 2], NULL, check_subgrid, &params[i + 2]);
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Check results from all threads
    bool is_valid = true;
    for (int i = 0; i < 11; i++) {
        if (valid[i] == 0) {             // If any thread found an invalid section
            is_valid = false;
            break;
        }
    }
    
    return is_valid;
}

/**
 * @brief Main function to demonstrate Sudoku validation
 *
 * Tests both a valid and an invalid Sudoku solution.
 *
 * @return 0 on successful execution
 */
int main() {
    printf("\n=== Testing Valid Sudoku Solution ===\n");
    if (validate_sudoku(valid_sudoku)) {
        printf("✓ Valid Sudoku solution is correctly identified as VALID!\n");
    } else {
        printf("✗ Error: Valid Sudoku solution incorrectly identified as invalid!\n");
    }
    
    printf("\n=== Testing Invalid Sudoku Solution ===\n");
    if (validate_sudoku(invalid_sudoku)) {
        printf("✗ Error: Invalid Sudoku solution incorrectly identified as valid!\n");
    } else {
        printf("✓ Invalid Sudoku solution is correctly identified as INVALID!\n");
    }
    
    return 0;
}