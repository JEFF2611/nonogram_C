
/**
 * Initialize a 2D board with the specified number of rows and columns
 * @param rows Number of rows
 * @param cols Number of columns
 * @return A 2D board with the specified number of rows and columns
 */
int** initialize_board(int rows, int cols) {
    int** board = (int**)malloc(rows * sizeof(int*));
    if (!board) {
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < rows; ++i) {
        board[i] = (int*)calloc(cols, sizeof(int));
        if (!board[i]) {
            // Cleanup previously allocated memory
            for (int j = 0; j < i; ++j) {
                free(board[j]);
            }
            free(board);
            return NULL; // Memory allocation failed
        }
    }

    return board;
}

/**
 * Read a JSON file and create a NonogramHints object
 * @param filename
 * @return
 */
NonoGramHints* read_json_file(const char* filename, int** board)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    // Read file content
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *) malloc(file_size + 1);
    if (!content) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return NULL;
    }

    if (fread(content, 1, file_size, file) != file_size) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        fclose(file);
        free(content);
        return NULL;
    }
    fclose(file);
    content[file_size] = '\0';

    // Parse the JSON string into a cJSON object
    cJSON *json = cJSON_Parse(content);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    // Extract the rows and cols arrays
    cJSON *rows_array = cJSON_GetObjectItemCaseSensitive(json, "rows");
    cJSON *cols_array = cJSON_GetObjectItemCaseSensitive(json, "cols");

    // Check if rows and cols are arrays
    if (!cJSON_IsArray(rows_array) || !cJSON_IsArray(cols_array)) {
        fprintf(stderr, "Error: rows or cols is not an array\n");
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    // Determine the number of rows and columns
    int rows_count = cJSON_GetArraySize(rows_array);
    int cols_count = cJSON_GetArraySize(cols_array);

    // Create NonogramHints object
    NonoGramHints *hints = nonogram_hints_create(board, rows_count, cols_count);
    if (!hints) {
        fprintf(stderr, "Error creating NonogramHints object\n");
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    // Extract and fill rows hints
    cJSON *rows_json = cJSON_GetObjectItemCaseSensitive(json, "rows");
    if (cJSON_IsArray(rows_json)) {
        int row_index = 0;
        cJSON *row_item;
        cJSON_ArrayForEach(row_item, rows_json)
        {
            int col_index = 0;
            cJSON *col_item;
            cJSON_ArrayForEach(col_item, row_item)
            {
                if (!cJSON_IsNumber(col_item)) {
                    fprintf(stderr, "Error: rows hint is not a number\n");
                    nonogram_hints_destroy(hints);
                    cJSON_Delete(json);
                    free(content);
                    return NULL;
                }
                hints->rows[row_index][col_index++] = col_item->valueint;
            }
            row_index++;
        }
    } else {
        fprintf(stderr, "Error: rows hint is not an array\n");
        nonogram_hints_destroy(hints);
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    // Extract and fill cols hints
    cJSON *cols_json = cJSON_GetObjectItemCaseSensitive(json, "cols");
    if (cJSON_IsArray(cols_json)) {
        int col_index = 0;
        cJSON *col_item;
        cJSON_ArrayForEach(col_item, cols_json)
        {
            int row_index = 0;
            cJSON *row_item;
            cJSON_ArrayForEach(row_item, col_item)
            {
                if (!cJSON_IsNumber(row_item)) {
                    fprintf(stderr, "Error: cols hint is not a number\n");
                    nonogram_hints_destroy(hints);
                    cJSON_Delete(json);
                    free(content);
                    return NULL;
                }
                hints->cols[col_index][row_index++] = row_item->valueint;
            }
            col_index++;
        }
    } else {
        fprintf(stderr, "Error: cols hint is not an array\n");
        nonogram_hints_destroy(hints);
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    // Clean up
    cJSON_Delete(json);
    free(content);
    return hints;

}

/**
 * Solves the nonogram recursively
 * @param board
 * @param row
 * @param col
 * @param rows
 * @param cols
 * @param hints
 * @return
 */
bool solve_recursive(int** board, int row, int col, int rows, int cols, NonoGramHints* hints)
{
    // Base case: If we reached the end of the board, check if it's solved
    if (row == rows) {
        return is_solved(board, rows, cols, hints);
    }

    // Calculate the next row and column
    int next_row = col == cols - 1 ? row + 1 : row;
    int next_col = (col + 1) % cols;

    // If the cell is already filled, move to the next cell
    if (board[row][col] == 1) {
        return solve_recursive(board, next_row, next_col, rows, cols, hints);
    }

    // Try filling the cell with 1 and check if it's a valid move
    board[row][col] = 1;
    if (is_valid_move(board, row, col, hints)) {
        if (solve_recursive(board, next_row, next_col, rows, cols, hints)) {
            return true;
        }
    }

    // If filling with 1 didn't lead to a solution, try filling with 0
    board[row][col] = 0;
    if (is_valid_move(board, row, col, hints)) {
        if (solve_recursive(board, next_row, next_col, rows, cols, hints)) {
            return true;
        }
    }

    // If neither filling with 1 nor 0 leads to a solution, backtrack
    board[row][col] = -1; // Mark cell as visited during backtracking
    return false;
}

/**
 * Check if a move is valid
 * @param board The board
 * @param row The row index
 * @param col The column index
 * @param value The value to fill in the cell
 * @return True if the move is valid, false otherwise
 */
bool is_valid_move(int** board, int row, int col, NonoGramHints* hints) {
    // Check if placing a 1 at the specified position violates row hints
    int row_hint_index = 0;
    int count = 0;
    for (int c = 0; c < hints->cols_count; ++c) {
        if (board[row][c] == 1) {
            count++;
        } else if (count > 0) {
            if (count != hints->rows[row][row_hint_index]) {
                return false; // Violates row hint
            }
            count = 0;
            row_hint_index++;
        }
    }
    if (count > 0 && count != hints->rows[row][row_hint_index]) {
        return false; // Check last count if not already checked
    }

    // Check if placing a 1 at the specified position violates column hints
    int col_hint_index = 0;
    count = 0;
    for (int r = 0; r < hints->rows_count; ++r) {
        if (board[r][col] == 1) {
            count++;
        } else if (count > 0) {
            if (count != hints->cols[col][col_hint_index]) {
                return false; // Violates column hint
            }
            count = 0;
            col_hint_index++;
        }
    }
    if (count > 0 && count != hints->cols[col][col_hint_index]) {
        return false; // Check last count if not already checked
    }

    return true; // Valid move
}

/**
 * Check is a board is solved
 * @param board
 * @param rows
 * @param cols
 * @param hints
 * @return
 */
bool is_solved(int** board, int rows, int cols, NonoGramHints* hints) {
    // Check if all rows satisfy their hints
    for (int row = 0; row < rows; ++row) {
        for (int i = 0, j = 0; i < hints->cols_count; ++i) {
            int count = 0;
            while (j < cols && board[row][j] == 1) {
                ++count;
                ++j;
            }
            if (count != hints->rows[row][i]) {
                return false;
            }
            if (j < cols && board[row][j] == 0) {
                ++j;
            }
        }
    }

    // Check if all columns satisfy their hints
    for (int col = 0; col < cols; ++col) {
        for (int i = 0, j = 0; i < hints->rows_count; ++i) {
            int count = 0;
            while (j < rows && board[j][col] == 1) {
                ++count;
                ++j;
            }
            if (count != hints->cols[col][i]) {
                return false;
            }
            if (j < rows && board[j][col] == 0) {
                ++j;
            }
        }
    }

    // If all conditions are satisfied, the puzzle is solved
    return true;
}

/**
 * Print the solution
 * @param board
 * @param rows
 * @param cols
 */
void print_solution(int** board, int rows, int cols) {
    // Print the top border
    printf("+");
    for (int i = 0; i < cols; ++i) {
        printf("-");
    }
    printf("+\n");

    // Print the board
    for (int i = 0; i < rows; ++i) {
        printf("|");
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == 1) {
                printf("#"); // Filled cell
            } else {
                printf(" "); // Empty cell
            }
        }
        printf("|\n");
    }

    // Print the bottom border
    printf("+");
    for (int i = 0; i < cols; ++i) {
        printf("-");
    }
    printf("+\n");
}

/**
 * Solve a nonogram puzzle
 * @param hints The nonogram hints object
 */
void solve_nonogram(NonoGramHints* hints) {
    int rows = nonogram_hints_get_rows_count(hints);
    int cols = nonogram_hints_get_cols_count(hints);

    int** board = (int**)malloc(rows * sizeof(int*));
    if (!board) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Initialize board with all cells set to -1 (unfilled)
    for (int i = 0; i < rows; ++i) {
        board[i] = (int*)malloc(cols * sizeof(int));
        if (!board[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            // Clean up memory for already allocated rows
            for (int j = 0; j < i; ++j) {
                free(board[j]);
            }
            free(board);
            return;
        }
        for (int j = 0; j < cols; ++j) {
            board[i][j] = -1;
        }
    }

    // Solve the nonogram puzzle
    if (!solve_recursive(board, 0, 0, rows, cols, hints)) {
        fprintf(stderr, "Unsolvable puzzle\n");
        // Clean up memory
        for (int i = 0; i < rows; ++i) {
            free(board[i]);
        }
        free(board);
        return;
    }

    // Print the first solution found
    print_solution(board, rows, cols);

    // Clean up memory
    for (int i = 0; i < rows; ++i) {
        free(board[i]);
    }
    free(board);
}


extern int** initialize_board(int rows, int cols);
extern NonoGramHints* read_json_file(const char* filename, int** board);
extern void solve_nonogram(NonoGramHints* hints);
extern bool is_valid_move(int** board, int row, int col, NonoGramHints* hints);
extern bool solve_recursive(int** board, int row, int col, int rows, int cols, NonoGramHints* hints);
extern bool is_solved(int** board, int rows, int cols, NonoGramHints* hints);
extern void print_solution(int** board, int rows, int cols);



//
// Created by engouan on 30/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#include "./cJSON.h"
#include "./nonogram.h"
#include "./pnmio.h" //PBM file

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s hints.json [--board board.pbm] [--output solved.pbm]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* hints_file = argv[1];
    const char* board_file = NULL;
    int** board_data = NULL;
    const char* output_file = NULL;

    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--board") == 0 && i + 1 < argc) {
            board_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        }
    }

    //Replace the standard output with the output file with dup2
    if (output_file) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
            fprintf(stderr, "Error opening output file: %s\n", output_file);
            return EXIT_FAILURE;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    //if the parameter board_file is not NULL, read the PBM board_file and store it in board_data, as an int**
    if (board_file) {
        FILE* f = fopen(board_file, "r");
        if (!f) {
            fprintf(stderr, "Error opening board file: %s\n", board_file);
            return EXIT_FAILURE;
        }
        int xdim, ydim, is_ascii;
        int type = get_pnm_type(f);
        if (type == PBM_ASCII) {
            read_pbm_header(f, &xdim, &ydim, &is_ascii);
            board_data = (int**)malloc(ydim * sizeof(int*));
            for (int i = 0; i < ydim; i++) {
                board_data[i] = (int*)malloc(xdim * sizeof(int));
                read_pbm_data(f, board_data[i], is_ascii);
            }
        } else {
            fprintf(stderr, "Error: Board file must be a PBM file\n");
            return EXIT_FAILURE;
        }
        fclose(f);
    }
        //else initialize board_data as an int**, with the dimensions of the hints
    else {
        NonoGramHints* hints = read_json_file(hints_file, NULL);
        if (!hints) {
            fprintf(stderr, "Error reading Nonogram puzzle from file: %s\n", hints_file);
            return EXIT_FAILURE;
        }
        board_data = initialize_board(nonogram_hints_get_rows_count(hints), nonogram_hints_get_cols_count(hints));
        nonogram_hints_destroy(hints);
    }


    // Open the input JSON file
    NonoGramHints* hints = read_json_file(hints_file, board_data);
    if (!hints) {
        fprintf(stderr, "Error reading Nonogram puzzle from file: %s\n", hints_file);
        return EXIT_FAILURE;
    }

    // Solve Nonogram puzzle
    solve_nonogram(hints);

    // Cleanup
    nonogram_hints_destroy(hints);
    free(board_data);

    return EXIT_SUCCESS;
}