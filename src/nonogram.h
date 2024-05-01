#ifndef NONOGRAM_H_
#define NONOGRAM_H_
// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.

#include <stdbool.h>
#include "./pnmio.h"

/**
 * NonoGramHints is a opaque structure that represents the hints of a nonogram.
 */
typedef struct _NonoGramHints NonoGramHints;

/**
 * @brief Create a new nonogram hints object
 * @param board The board
 * @param rows_count Number of rows in the board
 * @param cols_count Number of columns in the board
 * @return A new nonogram hints object
 * @note The board is a 2D array of 0s and 1s
 */
extern NonoGramHints *nonogram_hints_create(
  int **board,
  int rows_count,
  int cols_count
);
/**
 * @brief Destroy a nonogram hints object
 * @param hints The nonogram hints object
 * @note This function frees the memory allocated for the nonogram hints object
 */
extern void nonogram_hints_destroy(NonoGramHints *hints);

/**
 * @brief Get the number of rows in the nonogram hints object
 * @param hints The nonogram hints object
 * @return The number of rows in the nonogram hints object
 * @note This function returns the number of rows in the nonogram hints object
 */
extern int nonogram_hints_get_rows_count(NonoGramHints *hints);
/**
 * @brief Get the number of cols in the nonogram hints object
 * @param hints The nonogram hints object
 * @return The number of cols in the nonogram hints object
 * @note This function returns the number of cols in the nonogram hints object
 */
extern int nonogram_hints_get_cols_count(NonoGramHints *hints);
/**
 * @brief Get a specified hint in a row of the nonogram hints object
 * @param hints The nonogram hints object
 * @param row The row index
 * @param index The hint index
 * @return A specified hint in a row of the nonogram hints object
 * @note This function returns a specified hint in a row of the nonogram hints object
 */
extern int nonogram_hints_get_row_value(
  NonoGramHints *hints,
  int row,
  int index
);
/**
 * @brief Get a specified hint in a col of the nonogram hints object
 * @param hints The nonogram hints object
 * @param col The col index
 * @param index The hint index
 * @return A specified hint in a col of the nonogram hints object
 * @note This function returns a specified hint in a col of the nonogram hints object
 */
extern int nonogram_hints_get_col_value(
  NonoGramHints *hints,
  int col,
  int index
);

/**
 * @brief Convert a nonogram hints object to a string
 * @param hints The nonogram hints object
 * @return A string representation of the nonogram hints object
 * @note The string representation of the nonogram hints object is a JSON object
 * @note This function returns a pointer to a static buffer
 * @note The caller should copy the string if it needs to be used after calling this function again
 * @note The caller should call the function with a NULL argument to free the static buffer
 */
extern const char *nonogram_hints_to_string(NonoGramHints *hints);


#endif  // NONOGRAM_H_
