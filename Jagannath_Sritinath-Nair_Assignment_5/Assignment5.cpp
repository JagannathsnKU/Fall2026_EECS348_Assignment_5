/*
Program Name: Assignment 5 - Sudoku Solver (Batch File Version)
Description: This program solves five Sudoku puzzles using recursion and backtracking.
             It reads five puzzle files (puzzle1.txt through puzzle5.txt), finds 
             EVERY solution for each puzzle, and writes the results to matching 
             solution files (solution1.txt through solution5.txt).

Inputs: puzzle1.txt, puzzle2.txt, puzzle3.txt, puzzle4.txt, puzzle5.txt
Outputs: solution1.txt, solution2.txt, solution3.txt, solution4.txt, solution5.txt
         - If no solutions are found, outputs a single line: "No solution found."
         - If solutions are found, outputs all solutions in grid format followed by 
           a final line: "Solutions found: n"

Original Author: Saylor Statler
Original Date Created: October 12, 2026

Revised by: Jagannath Sritinath Nair
Revision Date: October 2026

Sources/Collaborators:
- Original Assignment 4 codebase by Saylor Statler
- Baseline architecture selected from Claude (Anthropic)
- ChatGPT and Claude used for analysis and verification
*/

#include <iostream>   // Include standard input/output stream library for console logging
#include <fstream>    // Include file stream library for reading input and writing solution files
#include <string>     // Include string library for file name manipulation

using namespace std;  // Use standard namespace to avoid prefixing std:: on stream objects

/**
 * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
 * Other Collaborators: Saylor Statler (Assignment 4 base code)
 **/
class SudokuSolver // Define the core object-oriented solver class
{ // Open class definition body
private: // Begin private member variables and helper functions
    char board[9][9];      // 9x9 character array storing current board state ('1'-'9' or '_')
    int solutionCount;     // Integer counter tracking total number of valid solutions found
    ofstream outFile;      // Output file stream object bound to the active solution file

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 base code)
     **/
    // Check whether a specific digit can legally be placed in a given cell
    bool isValid(int row, int col, char num) // Method taking target row, column, and character value
    { // Open function body for rule validation
        // Scan the entire row and column simultaneously for duplicate values
        for (int i = 0; i < 9; i++) // Loop through each index from 0 to 8
        { // Open row/column loop body
            // If the number already exists in this row, placement is invalid
            if (board[row][i] == num) // Check if row cell matches candidate number
                return false; // Return false because row constraint is violated

            // If the number already exists in this column, placement is invalid
            if (board[i][col] == num) // Check if column cell matches candidate number
                return false; // Return false because column constraint is violated
        } // Close row/column loop body

        // Determine the upper-left starting coordinate of the enclosing 3x3 block
        int startRow = (row / 3) * 3; // Calculate top row index of the 3x3 subgrid
        int startCol = (col / 3) * 3; // Calculate left column index of the 3x3 subgrid

        // Iterate through all 9 cells of the corresponding 3x3 subgrid
        for (int r = startRow; r < startRow + 3; r++) // Loop through 3 rows of the block
        { // Open block row loop body
            for (int c = startCol; c < startCol + 3; c++) // Loop through 3 columns of the block
            { // Open block column loop body
                // If the number exists within the 3x3 block, placement is invalid
                if (board[r][c] == num) // Check if subgrid cell matches candidate number
                    return false; // Return false because 3x3 box constraint is violated
            } // Close block column loop body
        } // Close block row loop body

        // Number violates no Sudoku rules; placement is legally allowed
        return true; // Return true indicating move is valid
    } // Close function body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 base code)
     **/
    // Verify that initial puzzle clues do not violate any Sudoku rules
    bool initialPuzzleValid() // Method returning boolean status of starting clues
    { // Open function body
        for (int r = 0; r < 9; r++) // Loop through all board rows
        { // Open outer grid loop
            for (int c = 0; c < 9; c++) // Loop through all board columns
            { // Open inner grid loop
                // Only validate pre-filled clue cells (non-underscore)
                if (board[r][c] != '_') // Check if cell contains a pre-set digit
                { // Open condition body
                    char num = board[r][c]; // Store the original character clue

                    // Temporarily clear the cell so it doesn't conflict with itself
                    board[r][c] = '_'; // Blank out cell temporarily in board array
                    bool valid = isValid(r, c, num); // Test validity of placement without self-conflict
                    // Restore the original clue value back to the board
                    board[r][c] = num; // Put original digit back into board array

                    // If a clue conflicts with another initial clue, puzzle is invalid
                    if (!valid) // Check if validation failed
                        return false; // Return false because starting puzzle is flawed
                } // Close condition body
            } // Close inner grid loop
        } // Close outer grid loop

        // Initial configuration is clean and valid
        return true; // Return true confirming starting board is legal
    } // Close function body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 base code)
     **/
    // Print the board to the console with 3x3 grid dividers for readability
    void printBoard() const // Constant method displaying board state to console
    { // Open function body
        for (int r = 0; r < 9; r++) // Loop through each row of the board
        { // Open row loop body
            // Insert horizontal dividers between 3x3 block rows
            if (r > 0 && r % 3 == 0) // Check if current row is a multiple of 3
                cout << "------+-------+------\n"; // Print horizontal separator line

            for (int c = 0; c < 9; c++) // Loop through each column in the row
            { // Open column loop body
                // Insert vertical dividers between 3x3 block columns
                if (c > 0 && c % 3 == 0) // Check if current column is a multiple of 3
                    cout << "| "; // Print vertical separator symbol

                cout << board[r][c] << " "; // Print character cell value followed by space
            } // Close column loop body

            cout << endl; // Print newline character at the end of each row
        } // Close row loop body
    } // Close function body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Jagannath Sritinath Nair (Stream output integration)
     **/
    // Write the current solved board to the solution file in 9x9 grid format
    void writeBoardToFile() // Method writing active board layout to open file stream
    { // Open function body
        for (int r = 0; r < 9; r++) // Loop through each row of the board array
        { // Open row loop body
            for (int c = 0; c < 9; c++) // Loop through each column of the board array
            { // Open column loop body
                outFile << board[r][c]; // Write current cell character to output stream

                // Separate cells with spaces, omitting the trailing space per line
                if (c < 8) // Check if column is before the final index
                    outFile << ' '; // Append separating space character
            } // Close column loop body

            outFile << '\n'; // Write newline character to output file after each row
        } // Close row loop body

        // Leave a blank line between multiple solutions for clean readability
        outFile << '\n'; // Append extra newline separator between multiple solution grids
    } // Close function body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 backtracking logic)
     **/
    // Recursively search for every possible solution using backtracking and MRV
    void solveRecursive() // Recursive method exploring solution space
    { // Open function body
        int bestRow = -1;          // Row index of most constrained empty cell initialized to -1
        int bestCol = -1;          // Column index of most constrained empty cell initialized to -1
        int fewestCandidates = 10; // Lowest candidate count initialized above max possible limit (9)

        // Scan grid using Minimum Remaining Values (MRV) heuristic
        for (int r = 0; r < 9; r++) // Loop through all rows to find empty cells
        { // Open outer scan loop
            for (int c = 0; c < 9; c++) // Loop through all columns to find empty cells
            { // Open inner scan loop
                if (board[r][c] == '_') // Check if cell is currently empty
                { // Open empty cell condition body
                    int candidates = 0; // Initialize candidate counter to zero for this cell

                    // Count how many legal digits fit in this empty cell
                    for (char num = '1'; num <= '9'; num++) // Loop through all possible digit characters '1' through '9'
                    { // Open digit check loop
                        if (isValid(r, c, num)) // Check if digit placement is legal
                            candidates++; // Increment valid candidate tally
                    } // Close digit check loop

                    // Dead end: zero legal options means this branch fails
                    if (candidates == 0) // Check if cell has zero valid choices
                        return; // Return immediately to trigger backtracking branch termination

                    // Track the cell with the fewest valid options
                    if (candidates < fewestCandidates) // Check if this cell is more constrained than previous ones
                    { // Open assignment body
                        fewestCandidates = candidates; // Update minimum candidate record
                        bestRow = r; // Store row coordinate of most constrained cell
                        bestCol = c; // Store column coordinate of most constrained cell
                    } // Close assignment body
                } // Close empty cell condition body
            } // Close inner scan loop
        } // Close outer scan loop

        // Base case: no empty cells remain, indicating a complete solution
        if (bestRow == -1) // Check if no empty cells were found
        { // Open base case body
            solutionCount++; // Increment total valid solutions counter by 1

            // Record solution to output file
            writeBoardToFile(); // Invoke helper method to write board state to solution file

            // Output solution notification to console
            cout << "\nSolution #" << solutionCount << ":\n"; // Print solution index header to console
            printBoard(); // Print solved board grid to console
            return; // Return from current recursive frame to continue searching for other solutions
        } // Close base case body

        // Recursive case: try every valid digit in the selected constrained cell
        for (char num = '1'; num <= '9'; num++) // Loop through digits '1' through '9' for the chosen cell
        { // Open recursive choice loop
            if (isValid(bestRow, bestCol, num)) // Check if candidate digit is valid in selected cell
            { // Open valid placement condition body
                // Place digit choice temporarily
                board[bestRow][bestCol] = num; // Assign candidate digit to board cell

                // Recurse deeper to solve remaining board state
                solveRecursive(); // Recursive function call advancing to next decision level

                // Backtrack: clear cell so subsequent digits can be tested
                board[bestRow][bestCol] = '_'; // Reset board cell back to underscore for backtracking
            } // Close valid placement condition body
        } // Close recursive choice loop
    } // Close function body

public: // Begin public member methods accessible outside the class
    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 base code)
     **/
    // Constructor initializes board to empty underscores and zero solutions
    SudokuSolver() // Class constructor method
    { // Open constructor body
        solutionCount = 0; // Set initial solution count to zero

        for (int r = 0; r < 9; r++) // Loop through all rows to clear board
        { // Open constructor row loop
            for (int c = 0; c < 9; c++) // Loop through all columns to clear board
            { // Open constructor column loop
                board[r][c] = '_'; // Initialize every grid position to underscore character
            } // Close constructor column loop
        } // Close constructor row loop
    } // Close constructor body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Saylor Statler (Assignment 4 validation checks)
     **/
    // Load and rigorously validate puzzle input file
    bool loadPuzzle(const string& filename) // Method taking filename string and returning boolean success status
    { // Open function body
        ifstream file(filename); // Open input file stream with specified filename

        // Verify file stream opened successfully
        if (!file.is_open()) // Check if file stream failed to open
        { // Open error condition body
            cout << "Error: Cannot open " << filename << endl; // Log error message to console
            return false; // Return false indicating file load failure
        } // Close error condition body

        // Read exactly 81 puzzle grid cells
        for (int r = 0; r < 9; r++) // Loop through 9 rows of the input grid
        { // Open file read row loop
            for (int c = 0; c < 9; c++) // Loop through 9 columns of the input grid
            { // Open file read column loop
                if (!(file >> board[r][c])) // Attempt to extract character token from file stream
                { // Open incomplete file error body
                    cout << "Error: Incomplete puzzle file: " << filename << endl; // Log incomplete file error
                    return false; // Return false indicating file contains fewer than 81 tokens
                } // Close incomplete file error body

                char ch = board[r][c]; // Store extracted character for validation

                // Validate that characters are strictly digits '1'-'9' or '_'
                if ((ch < '1' || ch > '9') && ch != '_') // Check if character is outside allowed set
                { // Open invalid character error body
                    cout << "Error: Invalid character in " << filename << endl; // Log invalid character error
                    return false; // Return false indicating malformed file content
                } // Close invalid character error body
            } // Close file read column loop
        } // Close file read row loop

        // Ensure no extra trailing non-whitespace characters exist in file
        char extra; // Character variable to check for trailing junk data
        if (file >> extra) // Check if stream extraction succeeds past 81 tokens
        { // Open extra character error body
            cout << "Error: Puzzle contains extra characters: " << filename << endl; // Log extra character error
            return false; // Return false indicating file has extra trailing data
        } // Close extra character error body

        return true; // Return true confirming file loaded and validated successfully
    } // Close function body

    /**
     * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
     * Other Collaborators: Jagannath Sritinath Nair (Batch file output integration)
     **/
    // Process an individual puzzle file, solve it, and write formatted results
    void solve(const string& puzzleFile, const string& solutionFile) // Method taking input and output filenames
    { // Open function body
        solutionCount = 0; // Reset solution counter for current puzzle instance

        cout << "\n====================================\n"; // Print visual divider header to console
        cout << "Puzzle File:   " << puzzleFile << endl; // Log input file name
        cout << "Solution File: " << solutionFile << endl; // Log output file name
        cout << "====================================\n"; // Print visual divider footer

        // Open the matching output file stream
        outFile.open(solutionFile); // Open ofstream bound to output filename

        if (!outFile.is_open()) // Verify output file stream opened successfully
        { // Open output stream error body
            cout << "Error: Cannot create " << solutionFile << endl; // Log creation error to console
            return; // Return early if file cannot be created
        } // Close output stream error body

        // Handle file loading failure per assignment specifications
        if (!loadPuzzle(puzzleFile)) // Call loadPuzzle and check for failure
        { // Open load failure body
            cout << "Skipping invalid file.\n"; // Log skip notification to console
            outFile << "No solution found." << endl; // Write required fallback string to output file
            outFile.close(); // Close output file stream
            return; // Return early from solve method
        } // Close load failure body

        cout << "\nOriginal Puzzle:\n"; // Print original puzzle header to console
        printBoard(); // Display original board state in console

        // Handle invalid initial puzzle clues per specifications
        if (!initialPuzzleValid()) // Check if starting clues violate Sudoku rules
        { // Open initial conflict body
            cout << "\nInvalid puzzle.\n"; // Log invalid puzzle message to console
            cout << "No solution found.\n"; // Log failure notification to console
            outFile << "No solution found." << endl; // Write required fallback string to output file
            outFile.close(); // Close output file stream
            return; // Return early from solve method
        } // Close initial conflict body

        // Execute recursive backtracking solver
        solveRecursive(); // Initiate recursive solver execution

        // Output final status messages to console and required format to file
        if (solutionCount == 0) // Check if zero solutions were discovered
        { // Open zero solution body
            cout << "\nNo solution found.\n"; // Log zero solution status to console
            outFile << "No solution found." << endl; // Write required fallback string to output file
        } // Close zero solution body
        else // Handle case where one or more solutions were found
        { // Open solutions found body
            cout << "\nTotal solutions found: " << solutionCount << endl; // Log total solution count to console
            outFile << "Solutions found: " << solutionCount << endl; // Write required total solutions footer to output file
        } // Close solutions found body

        // Close output stream
        outFile.close(); // Close active file stream handler to save data to disk
    } // Close function body
}; // Close class definition

/**
 * Source: Claude 3.5 Sonnet (modified by author for robust token extraction)
 * Other Collaborators: Saylor Statler (Assignment 4 main loop structure)
 **/
// Main execution loop batch-processing all five required puzzles
int main() // Main entry point function returning integer status
{ // Open main function body
    // Define input array for the five required puzzle files
    string puzzleFiles[5] = { // Array holding five input file name strings
        "puzzle1.txt", // String for first puzzle file
        "puzzle2.txt", // String for second puzzle file
        "puzzle3.txt", // String for third puzzle file
        "puzzle4.txt", // String for fourth puzzle file
        "puzzle5.txt"  // String for fifth puzzle file
    }; // Close input array initialization

    // Define corresponding output array for the five solution files
    string solutionFiles[5] = { // Array holding five corresponding output file name strings
        "solution1.txt", // String for first solution output file
        "solution2.txt", // String for second solution output file
        "solution3.txt", // String for third solution output file
        "solution4.txt", // String for fourth solution output file
        "solution5.txt"  // String for fifth solution output file
    }; // Close output array initialization

    // Iterate through all five puzzles sequentially using fresh solver instances
    for (int i = 0; i < 5; i++) // Loop from index 0 through 4
    { // Open batch loop body
        SudokuSolver solver; // Instantiate a fresh SudokuSolver object for each puzzle file
        solver.solve(puzzleFiles[i], solutionFiles[i]); // Invoke solve method passing corresponding input and output files
    } // Close batch loop body

    cout << "\nAll five puzzle files processed successfully.\n"; // Log batch completion message to console

    return 0; // Return 0 to operating system indicating successful execution
} // Close main function body
