// Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
// Licensed under the BSD-3 License. See the LICENSE file for details.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

#include "./nonogram.h"

int main(void) {
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
  printf("%s\n", nonogram_hints_to_string(hints));
  assert(strcmp(nonogram_hints_to_string(hints),
                "{\"rows\":[[2,1],[1,1],[],[1,1],[2,2]],"
                "\"cols\":[[2,2],[1,1],[],[1,1],[1,2]]}") == 0);
  nonogram_hints_to_string(NULL);
  nonogram_hints_destroy(hints);
  for (int row = 0; row < rows_count; row++) {
    free(board[row]);
  }
  free(board);

  return EXIT_SUCCESS;
}
