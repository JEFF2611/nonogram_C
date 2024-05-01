// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.
#include "./nonogram.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./nonogram.inc"
#include "./cJSON.h"

/**
 * @brief Allocate a nonogram hints object
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return A new nonogram hints object
 * @note This function allocates a nonogram hints object
 */
static NonoGramHints *_nonogram_hints_new(int rows_count, int cols_count) {
  if (!rows_count || !cols_count) {
    // Return NULL if the number of rows or columns is 0
    return NULL;
  }
  NonoGramHints *hints = malloc(sizeof(NonoGramHints));
  if (!hints) {
    // Return NULL if the memory allocation failed
    return NULL;
  }
  hints->rows = malloc(rows_count * sizeof(int *));
  if (!hints->rows) {
    // Free the memory allocated for the nonogram hints object
    free(hints);
    // Return NULL if the memory allocation failed
    return NULL;
  }
  hints->cols = malloc(cols_count * sizeof(int *));
  if (!hints->cols) {
    // Free the memory allocated for the rows hints and the nonogram hints
    // object
    free(hints->rows);
    free(hints);
    // Return NULL if the memory allocation failed
    return NULL;
  }
  for (int row = 0; row < rows_count; row++) {
    // Allocate the memory for the rows hints
    hints->rows[row] = calloc(cols_count, sizeof(int));
    if (!hints->rows[row]) {
      for (int i = 0; i < row; i++) {
        // Free the memory allocated for the rows hints if the memory allocation
        // failed
        free(hints->rows[i]);
      }
      // Free the memory allocated for the rows hints and the nonogram hints
      // object
      free(hints->rows);
      free(hints->cols);
      free(hints);
      // Return NULL if the memory allocation failed
      return NULL;
    }
  }
  for (int col = 0; col < cols_count; col++) {
    hints->cols[col] = calloc(rows_count, sizeof(int));
    if (!hints->cols[col]) {
      for (int i = 0; i < col; i++) {
        // Free the memory allocated for the cols hints if the memory allocation
        // failed
        free(hints->cols[i]);
      }
      for (int row = 0; row < rows_count; row++) {
        free(hints->rows[row]);
      }
      // Free the memory allocated for the cols hints and the nonogram hints
      // object
      free(hints->rows);
      free(hints->cols);
      free(hints);
      // Return NULL if the memory allocation failed
      return NULL;
    }
  }
  // Return the nonogram hints object
  return hints;
}

/**
 * @brief Fill a nonogram hints object
 * @param hints The nonogram hints object
 * @param board The board
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return The nonogram hints object
 * @note This function fills a nonogram hints object
 * @note The board is a 2D array of 0s and 1s
 * @note The nonogram hints object is filled with the hints for the rows and
 * columns of the board
 * @note The hints are the number of consecutive 1s in the rows and columns of
 * the board
 * @note The hints are stored in the nonogram hints object as 2D arrays of
 * integers
 */
static NonoGramHints *_nonogram_hints_fill(
  NonoGramHints *hints,
  int **board,
  int rows_count,
  int cols_count
) {
  // Fill the rows and cols count
  hints->rows_count = rows_count;
  hints->cols_count = cols_count;
  // Fill the rows hints
  for (int row = 0; row < rows_count; row++) {
    int count = 0;
    int index = 0;
    for (int col = 0; col < cols_count; col++) {
      // Count the number of consecutive 1s in the rows
      if (board[row][col] == 1) {
        count++;
      } else {
        if (count > 0) {
          hints->rows[row][index++] = count;
          count = 0;
        }
      }
    }
    hints->rows[row][index] = count;
  }
  // Fill the cols hints
  for (int col = 0; col < cols_count; col++) {
    int count = 0;
    int index = 0;
    for (int row = 0; row < rows_count; row++) {
      // Count the number of consecutive 1s in the cols
      if (board[row][col] == 1) {
        count++;
      } else {
        if (count > 0) {
          hints->cols[col][index++] = count;
          count = 0;
        }
      }
    }
    hints->cols[col][index++] = count;
  }
  return hints;
}

NonoGramHints *nonogram_hints_create(
  int **board,
  int rows_count,
  int cols_count
) {
  // Create a new nonogram hints object
  NonoGramHints *hints = _nonogram_hints_new(rows_count, cols_count);
  if (hints) {
    // Fill the nonogram hints object
    _nonogram_hints_fill(hints, board, rows_count, cols_count);
  }
  return hints;
}

void nonogram_hints_destroy(NonoGramHints *hints) {
  // Free the memory allocated for the rows hints
  for (int row = 0; row < hints->rows_count; row++) {
    free(hints->rows[row]);
  }
  // Free the memory allocated for the cols hints
  for (int col = 0; col < hints->cols_count; col++) {
    free(hints->cols[col]);
  }
  // Free the memory allocated for the nonogram hints object
  free(hints->rows);
  free(hints->cols);
  free(hints);
}

int nonogram_hints_get_rows_count(NonoGramHints *hints) {
  // Return the number of rows in the nonogram hints object
  return hints->rows_count;
}

int nonogram_hints_get_cols_count(NonoGramHints *hints) {
  // Return the number of cols in the nonogram hints object
  return hints->cols_count;
}

int nonogram_hints_get_row_value(NonoGramHints *hints, int row, int index) {
  assert(row < hints->rows_count);
  assert(index < hints->cols_count);
  // Return a specified hint in a row of the nonogram hints object
  return hints->rows[row][index];
}

int nonogram_hints_get_col_value(NonoGramHints *hints, int col, int index) {
  assert(col < hints->cols_count);
  assert(index < hints->rows_count);
  // Return a specified hint in a col of the nonogram hints object
  return hints->cols[col][index];
}

/**
 * @brief Concat a string to an  existent string
 * @param pstring A pointer to the existent string
 * @param plength A pointer to the length of the existent string
 * @param add The string to add
 * @return true if the string was added, false otherwise
 * @note This function adds a string to a string
 */
static bool _add_string(
  char **pstring,
  unsigned int *plength,
  const char *add
) {
  if (add) {
    // Allocate the memory for the string
    unsigned int inc_length = strlen(add);
    char *string = realloc(*pstring, *plength + inc_length + 1);
    if (string) {
      // Add the string to the existent string
      *pstring = string;
      strncpy(*pstring + *plength, add, inc_length + 1);
      *plength += inc_length;
      return true;
    } else {
      // Return false if the memory allocation failed
      return false;
    }
  } else {
    // Return false if the string to add is NULL
    return false;
  }
}

/**
 * @brief Add a string to an existent string
 * @param pstring A pointer to the existent string
 * @param plength A pointer to the length of the existent string
 * @param add The string to add
 * @return true if the string was added, false otherwise
 * @note This macro adds a string to a string and return NULL if the memory
 *       allocation failed
 */
#define _ADD_STRING(pstring, plength, add) \
  if (!_add_string(pstring, plength, add)) \
    return NULL;

const char *nonogram_hints_to_string(NonoGramHints *hints) {
  // Create a string representation of the nonogram hints object
  static char *string = NULL;
  char buffer[128];

  // Free the memory allocated for the string
  if (!hints) {
    free(string);
    string = NULL;
    return NULL;
  }

  int length = 0;

  // Add "{" to the string
  _ADD_STRING(&string, &length, "{");

  // Add the rows hints to the string
  _ADD_STRING(&string, &length, "\"rows\":[");

  for (int row = 0; row < hints->rows_count; row++) {
    _ADD_STRING(&string, &length, row > 0 ? "," : "");
    _ADD_STRING(&string, &length, "[");
    for (int index = 0; index < hints->cols_count; index++) {
      if (!hints->rows[row][index]) {
        break;
      }
      _ADD_STRING(&string, &length, index > 0 ? "," : "");
      snprintf(buffer, sizeof buffer, "%d", hints->rows[row][index]);
      _ADD_STRING(&string, &length, buffer);
    }
    _ADD_STRING(&string, &length, "]");
  }
  _ADD_STRING(&string, &length, "]");

  // Add the cols hints to the string
  _ADD_STRING(&string, &length, ",\"cols\":[");

  for (int col = 0; col < hints->cols_count; col++) {
    _ADD_STRING(&string, &length, col > 0 ? "," : "");
    _ADD_STRING(&string, &length, "[");
    for (int index = 0; index < hints->rows_count; index++) {
      if (!hints->cols[col][index]) {
        break;
      }
      _ADD_STRING(&string, &length, index > 0 ? "," : "");
      snprintf(buffer, sizeof buffer, "%d", hints->cols[col][index]);
      _ADD_STRING(&string, &length, buffer);
    }
    _ADD_STRING(&string, &length, "]");
  }
  _ADD_STRING(&string, &length, "]");

  // Add "}" to the string
  _ADD_STRING(&string, &length, "}");

  return string;
}

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
    FILE *file = fopen(filename, "w+");
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

    // Parse JSON content
    cJSON *json = cJSON_Parse(content);
    if (!json) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(content);
        return NULL;
    }

    // Extract rows and cols counts
    cJSON *rows_count_json = cJSON_GetObjectItemCaseSensitive(json, "rows_count");
    cJSON *cols_count_json = cJSON_GetObjectItemCaseSensitive(json, "cols_count");
    if (!cJSON_IsNumber(rows_count_json) || !cJSON_IsNumber(cols_count_json)) {
        fprintf(stderr, "Error: rows_count or cols_count is not a number\n");
        cJSON_Delete(json);
        free(content);
        return NULL;
    }

    int rows_count = rows_count_json->valueint;
    int cols_count = cols_count_json->valueint;

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

