//
// Created by engouan on 30/04/2024.
//

#ifndef NONOGRAM_NONOGRAM_SOLVE_H
#define NONOGRAM_NONOGRAM_SOLVE_H

#endif //NONOGRAM_NONOGRAM_SOLVE_H

#include <stdbool.h>
#include "./nonogram.h"
#include "./nonogram.inc"

extern int** initialize_board(int rows, int cols);
extern NonoGramHints* read_json_file(const char* filename);
extern void solve_nonogram(NonoGramHints* hints);
extern bool is_valid_move(int** board, int row, int col, NonoGramHints* hints);
extern bool solve_recursive(int** board, int row, int col, int rows, int cols, NonoGramHints* hints);
extern bool is_solved(int** board, int rows, int cols, NonoGramHints* hints);
extern void print_solution(int** board, int rows, int cols);
