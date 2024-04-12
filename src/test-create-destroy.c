// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.
#include <stdlib.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./nonogram.h"
#include "./nonogram.inc"

int main(void) {
  assert(nonogram_hints_create(NULL, 0, 0) == NULL);
  int rows_count = 5;
  int cols_count = 5;
  int **board = malloc(rows_count * sizeof(int *));
  for (int row = 0; row < rows_count; row++) {
    board[row] = calloc(cols_count, sizeof(int));
  }
  /**
   * Board and hints:
   *      2 1   1 1
   *      2 1   1 2
   *     +---------
   * 2 1 |■ ■   ■
   * 1 1 |■       ■
   *     |
   * 1 1 |■       ■
   * 2 2 |■ ■   ■ ■
   */
  board[0][0] = 1;
  board[0][1] = 1;
  board[0][3] = 1;
  board[1][0] = 1;
  board[1][4] = 1;
  board[3][0] = 1;
  board[3][4] = 1;
  board[4][0] = 1;
  board[4][1] = 1;
  board[4][3] = 1;
  board[4][4] = 1;
  NonoGramHints *hints = nonogram_hints_create(board, rows_count, cols_count);
  assert(hints);
  assert(hints->rows);
  assert(hints->cols);
  assert(hints->rows_count == rows_count);
  assert(hints->cols_count == cols_count);
  assert(hints->rows[0][0] == 2);
  assert(hints->rows[0][1] == 1);
  assert(hints->rows[0][2] == 0);
  assert(hints->rows[1][0] == 1);
  assert(hints->rows[1][1] == 1);
  assert(hints->rows[1][2] == 0);
  assert(hints->rows[2][0] == 0);
  assert(hints->rows[3][0] == 1);
  assert(hints->rows[3][1] == 1);
  assert(hints->rows[3][2] == 0);
  assert(hints->rows[4][0] == 2);
  assert(hints->rows[4][1] == 2);
  assert(hints->rows[4][2] == 0);
  assert(hints->cols[0][0] == 2);
  assert(hints->cols[0][1] == 2);
  assert(hints->cols[0][2] == 0);
  assert(hints->cols[1][0] == 1);
  assert(hints->cols[1][1] == 1);
  assert(hints->cols[1][2] == 0);
  assert(hints->cols[2][0] == 0);
  assert(hints->cols[3][0] == 1);
  assert(hints->cols[3][1] == 1);
  assert(hints->cols[3][2] == 0);
  assert(hints->cols[4][0] == 1);
  assert(hints->cols[4][1] == 2);
  assert(hints->cols[4][2] == 0);
  nonogram_hints_destroy(hints);
  for (int row = 0; row < rows_count; row++) {
    free(board[row]);
  }
  free(board);

  return EXIT_SUCCESS;
}
