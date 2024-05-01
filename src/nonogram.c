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
