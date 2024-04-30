// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.

/**
 * NonoGramHints is a opaque structure that represents the hints of a nonogram.
 * @note This structure is defined in nonogram.inc
 * @note This structure is used to represent the hints of a nonogram
 */
struct _NonoGramHints {
  int rows_count;  // Number of rows in the board
  int cols_count;  // Number of columns in the board
  int **rows;      // Hints for the rows
  int **cols;      // Hints for the columns
};
