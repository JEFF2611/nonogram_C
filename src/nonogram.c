
// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.

/**
 * @file nonogram.c
 * @brief Implementation of nonogram functions.
 *
 * Detailed description of the file.
 */
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
 * 
 * This function allocates memory for a nonogram hints object, including
 * the memory required for row and column hints arrays.
 * 
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return A pointer to the newly allocated nonogram hints object, or NULL if 
 *         memory allocation fails or if rows_count or cols_count is zero.
 */
static NonoGramHints *_nonogram_hints_new(int rows_count, int cols_count) {
  if (!rows_count || !cols_count) {
    return NULL;
  }
  NonoGramHints *hints = malloc(sizeof(NonoGramHints));
  if (!hints) {
    return NULL;
  }
  hints->rows = malloc(rows_count * sizeof(int *));
  if (!hints->rows) {
    free(hints);
    return NULL;
  }
  hints->cols = malloc(cols_count * sizeof(int *));
  if (!hints->cols) {
    free(hints->rows);
    free(hints);
    return NULL;
  }
  for (int row = 0; row < rows_count; row++) {
    hints->rows[row] = calloc(cols_count, sizeof(int));
    if (!hints->rows[row]) {
      for (int i = 0; i < row; i++) {
        free(hints->rows[i]);
      }
      free(hints->rows);
      free(hints->cols);
      free(hints);
      return NULL;
    }
  }
  for (int col = 0; col < cols_count; col++) {
    hints->cols[col] = calloc(rows_count, sizeof(int));
    if (!hints->cols[col]) {
      for (int i = 0; i < col; i++) {
        free(hints->cols[i]);
      }
      for (int row = 0; row < rows_count; row++) {
        free(hints->rows[row]);
      }
      free(hints->rows);
      free(hints->cols);
      free(hints);
      return NULL;
    }
  }
  return hints;
}

/**
 * @brief Fill a nonogram hints object
 * 
 * This function fills the nonogram hints object with hints based on the given 
 * board. Hints represent the number of consecutive 1s in each row and column.
 * 
 * @param hints The nonogram hints object to fill
 * @param board The board represented as a 2D array of 0s and 1s
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return The filled nonogram hints object
 */
static NonoGramHints *_nonogram_hints_fill(
  NonoGramHints *hints,
  int **board,
  int rows_count,
  int cols_count
) {
  hints->rows_count = rows_count;
  hints->cols_count = cols_count;
  for (int row = 0; row < rows_count; row++) {
    int count = 0;
    int index = 0;
    for (int col = 0; col < cols_count; col++) {
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
  for (int col = 0; col < cols_count; col++) {
    int count = 0;
    int index = 0;
    for (int row = 0; row < rows_count; row++) {
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

/**
 * @brief Create a nonogram hints object from a board
 * 
 * This function creates and fills a nonogram hints object based on the provided 
 * board.
 * 
 * @param board The board represented as a 2D array of 0s and 1s
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return A pointer to the created nonogram hints object, or NULL if creation 
 *         or filling fails.
 */
NonoGramHints *nonogram_hints_create(
  int **board,
  int rows_count,
  int cols_count
) {
  NonoGramHints *hints = _nonogram_hints_new(rows_count, cols_count);
  if (hints) {
    _nonogram_hints_fill(hints, board, rows_count, cols_count);
  }
  return hints;
}

/**
 * @brief Destroy a nonogram hints object
 * 
 * This function frees the memory allocated for the nonogram hints object.
 * 
 * @param hints The nonogram hints object to destroy
 */
void nonogram_hints_destroy(NonoGramHints *hints) {
  for (int row = 0; row < hints->rows_count; row++) {
    free(hints->rows[row]);
  }
  for (int col = 0; col < hints->cols_count; col++) {
    free(hints->cols[col]);
  }
  free(hints->rows);
  free(hints->cols);
  free(hints);
}

/**
 * @brief Get the number of rows in a nonogram hints object
 * 
 * @param hints The nonogram hints object
 * @return The number of rows in the nonogram hints object
 */
int nonogram_hints_get_rows_count(NonoGramHints *hints) {
  return hints->rows_count;
}

/**
 * @brief Get the number of columns in a nonogram hints object
 * 
 * @param hints The nonogram hints object
 * @return The number of columns in the nonogram hints object
 */
int nonogram_hints_get_cols_count(NonoGramHints *hints) {
  return hints->cols_count;
}

/**
 * @brief Get a specific row hint value
 * 
 * @param hints The nonogram hints object
 * @param row The row index
 * @param index The hint index within the row
 * @return The specified hint value in the row
 */
int nonogram_hints_get_row_value(NonoGramHints *hints, int row, int index) {
  assert(row < hints->rows_count);
  assert(index < hints->cols_count);
  return hints->rows[row][index];
}

/**
 * @brief Get a specific column hint value
 * 
 * @param hints The nonogram hints object
 * @param col The column index
 * @param index The hint index within the column
 * @return The specified hint value in the column
 */
int nonogram_hints_get_col_value(NonoGramHints *hints, int col, int index) {
  assert(col < hints->cols_count);
  assert(index < hints->rows_count);
  return hints->cols[col][index];
}

/**
 * @brief Concat a string to an existent string
 * 
 * This function concatenates a given string to an existing string, reallocating 
 * memory as necessary.
 * 
 * @param pstring A pointer to the existent string
 * @param plength A pointer to the length of the existent string
 * @param add The string to add
 * @return true if the string was added successfully, false otherwise
 */
static bool _add_string(
  char **pstring,
  unsigned int *plength,
  const char *add
) {
  if (add) {
    unsigned int inc_length = strlen(add);
    char *string = realloc(*pstring, *plength + inc_length + 1);
    if (string) {
      *pstring = string;
      strncpy(*pstring + *plength, add, inc_length + 1);
      *plength += inc_length;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

/**
 * @brief Add a string to an existent string
 * 
 * This macro adds a string to an existing string and returns NULL if the memory 
 * allocation fails.
 * 
 * @param pstring A pointer to the existent string
 * @param plength A pointer to the length of the existent string
 * @param add The string to add
 */
#define _ADD_STRING(pstring, plength, add) \
  if (!_add_string(pstring, plength, add)) \
    return NULL;

/**
 * @brief Convert a nonogram hints object to a string
 * 
 * This function creates a string representation of the nonogram hints object.
 * 
 * @param hints The nonogram hints object
 * @return A string representation of the nonogram hints object, or NULL if the 
 *         hints object is NULL or if memory allocation fails.
 */
const char *nonogram_hints_to_string(NonoGramHints *hints) {
  static char *string = NULL;


  char buffer[128];

  if (!hints) {
    free(string);
    string = NULL;
    return NULL;
  }

  int length = 0;

  _ADD_STRING(&string, &length, "{");
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

  _ADD_STRING(&string, &length, "}");

  return string;
}
