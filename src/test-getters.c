// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.
#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./nonogram.h"

int main(void) {
  int rows_count = 4;
  int cols_count = 3;
  int **board = malloc(rows_count * sizeof(int *));
  for (int row = 0; row < rows_count; row++) {
    board[row] = calloc(cols_count, sizeof(int));
  }
  /**
   * Board and hints:
   *        1 2
   *      1 1 1
   *     +-----
   * 1 1 |■   ■
   *   2 |  ■ ■
   *     |
   *   2 |  ■ ■
   */
  board[0][0] = 1;
  board[0][2] = 1;
  board[1][1] = 1;
  board[1][2] = 1;
  board[3][1] = 1;
  board[3][2] = 1;

  NonoGramHints *hints = nonogram_hints_create(board, rows_count, cols_count);

  assert(nonogram_hints_get_rows_count(hints) == rows_count);
  assert(nonogram_hints_get_cols_count(hints) == cols_count);

  assert(nonogram_hints_get_row_value(hints, 0, 0) == 1);
  assert(nonogram_hints_get_row_value(hints, 0, 1) == 1);
  assert(nonogram_hints_get_row_value(hints, 0, 2) == 0);
  assert(nonogram_hints_get_row_value(hints, 1, 0) == 2);
  assert(nonogram_hints_get_row_value(hints, 1, 1) == 0);
  assert(nonogram_hints_get_row_value(hints, 1, 2) == 0);
  assert(nonogram_hints_get_row_value(hints, 2, 0) == 0);
  assert(nonogram_hints_get_row_value(hints, 2, 1) == 0);
  assert(nonogram_hints_get_row_value(hints, 2, 2) == 0);
  assert(nonogram_hints_get_row_value(hints, 3, 0) == 2);
  assert(nonogram_hints_get_row_value(hints, 3, 1) == 0);
  assert(nonogram_hints_get_row_value(hints, 3, 2) == 0);

  assert(nonogram_hints_get_col_value(hints, 0, 0) == 1);
  assert(nonogram_hints_get_col_value(hints, 0, 1) == 0);
  assert(nonogram_hints_get_col_value(hints, 0, 2) == 0);
  assert(nonogram_hints_get_col_value(hints, 0, 3) == 0);
  assert(nonogram_hints_get_col_value(hints, 1, 0) == 1);
  assert(nonogram_hints_get_col_value(hints, 1, 1) == 1);
  assert(nonogram_hints_get_col_value(hints, 1, 2) == 0);
  assert(nonogram_hints_get_col_value(hints, 1, 3) == 0);
  assert(nonogram_hints_get_col_value(hints, 2, 0) == 2);
  assert(nonogram_hints_get_col_value(hints, 2, 1) == 1);
  assert(nonogram_hints_get_col_value(hints, 2, 2) == 0);
  assert(nonogram_hints_get_col_value(hints, 2, 3) == 0);

  nonogram_hints_destroy(hints);
  for (int row = 0; row < rows_count; row++) {
    free(board[row]);
  }
  free(board);

  return EXIT_SUCCESS;
}
