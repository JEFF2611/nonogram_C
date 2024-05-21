//
// Created by engouan on 30/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "./cJSON.h"
#include "./nonogram.h"
#include "./nonogram.inc"
#include "./pnmio.h" //PBM file, read and write

#define MAX_HINTS 32

/**
 * @brief Create a board from a nonogram hints object
 * @param hints The nonogram hints object
 * @return A 2D array representing the game board
 * @note This function creates a game board from a nonogram hints object
 */
int **nonogram_board_create_from_hints(NonoGramHints *hints) {
    int rows_count = hints->rows_count;
    int cols_count = hints->cols_count;

    // Créer un tableau de jeu initialisé à 0
    int **board = (int **)malloc(rows_count * sizeof(int *));
    if (!board) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    for (int row = 0; row < rows_count; row++) {
        board[row] = (int *)calloc(cols_count, sizeof(int));
        if (!board[row]) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            // Libérer la mémoire allouée précédemment
            for (int i = 0; i < row; i++) {
                free(board[i]);
            }
            free(board);
            return NULL;
        }
    }

    for (int row = 0; row < rows_count; row++) {
        int col = 0;
        for (int i = 0; i < MAX_HINTS && hints->rows[row][i] != 0; i++) {
            int hint = hints->rows[row][i];
            // Remplir les blocs en vérifiant les colonnes et la ligne précédente
            for (int j = 0; j < hint; j++) {
                if (col < cols_count) {
                    // Vérifier si la cellule est déjà remplie par une colonne
                    if (board[row][col] == 0 && (row == 0 || board[row - 1][col] == 1)) {
                        board[row][col] = 1;
                    }
                    col++;
                }
            }
            // Ajouter un espace entre les blocs si c'est permis
            if (col < cols_count && i < MAX_HINTS - 1 && hints->rows[row][i + 1] != 0) {
                if (board[row][col] == 0) {
                    board[row][col] = 0;
                }
                col++;
            }
        }
    }


    return board;
}

/**
 * @brief Fill a row of the board
 * @param row_hints
 * @param row
 * @param cols_count
 */
void fill_row(int *row_hints, int *row, int cols_count) {
    int col = 0;
    for (int i = 0; i < MAX_HINTS && row_hints[i] != 0; i++) {
        int hint = row_hints[i];
        // Remplir les blocs en vérifiant les colonnes
        for (int j = 0; j < hint; j++) {
            if (col < cols_count) {
                row[col++] = 1;
            }
        }
        // Ajouter un espace entre les blocs si c'est permis
        if (col < cols_count && i < MAX_HINTS - 1 && row_hints[i + 1] != 0) {
            row[col++] = 0;
        }
    }
}

/**
 * @brief Update the columns of the board
 * @param board
 * @param cols_hints
 * @param rows_count
 * @param cols_count
 */
void update_cols(int **board, int **cols_hints, int rows_count, int cols_count) {
    for (int col = 0; col < cols_count; col++) {
        int filled_cells = 0;
        for (int row = 0; row < rows_count; row++) {
            if (board[row][col] == 1) {
                filled_cells++;
            }
        }

        int col_hint_idx = 0;
        int hint_filled = 0;
        for (int row = 0; row < rows_count; row++) {
            if (board[row][col] == 1) {
                hint_filled++;
            } else {
                if (hint_filled > 0 && hint_filled == cols_hints[col][col_hint_idx]) {
                    col_hint_idx++;
                }
                hint_filled = 0;
            }
        }
    }
}

/**
 * @brief Solve a nonogram
 * @param hints
 * @param board
 */
void solve_nonogram(NonoGramHints *hints, int **board) {
    for (int row = 0; row < hints->rows_count; row++) {
        fill_row(hints->rows[row], board[row], hints->cols_count);
        update_cols(board, hints->cols, hints->rows_count, hints->cols_count);
    }
}

/**
 * @brief Write a nonogram board to a PBM file
 * @param filename
 * @param board
 * @param rows_count
 * @param cols_count
 */
void nonogram_write_pbm(const char *filename, int **board, int rows_count, int cols_count) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for writing\n", filename);
        return;
    }

    // Write PBM header
    fprintf(file, "P1\n%d %d\n", cols_count, rows_count);

    // Write board data
    for (int row = 0; row < rows_count; row++) {
        for (int col = 0; col < cols_count; col++) {
            // If the cell is filled, write 1, otherwise write 0
            fprintf(file, "%d ", board[row][col] ? 1 : 0);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

/**
 * @brief Parse a JSON file containing nonogram hints
 * @param filename The JSON file to parse
 * @return A nonogram hints object
 */
NonoGramHints *parse_json(const char *filename) {
    // Ouvrir le fichier JSON
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    // Lire le contenu du fichier
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *json_content = (char *)malloc(file_size + 1);
    fread(json_content, 1, file_size, file);
    fclose(file);

    // Ajouter la terminaison nulle à la fin de la chaîne JSON
    json_content[file_size] = '\0';

    // Analyser le contenu JSON
    cJSON *root = cJSON_Parse(json_content);
    if (!root) {
        fprintf(stderr, "Error: Failed to parse JSON\n");
        free(json_content);
        return NULL;
    }

    // Extraire les indices de lignes et de colonnes
    cJSON *rows = cJSON_GetObjectItem(root, "rows");
    cJSON *cols = cJSON_GetObjectItem(root, "cols");
    if (!rows || !cols) {
        fprintf(stderr, "Error: Invalid JSON format\n");
        cJSON_Delete(root);
        free(json_content);
        return NULL;
    }

    // Créer un objet NonoGramHints
    NonoGramHints *hints = (NonoGramHints *)malloc(sizeof(NonoGramHints));
    if (!hints) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        cJSON_Delete(root);
        free(json_content);
        return NULL;
    }

    // Initialiser les compteurs de lignes et de colonnes
    hints->rows_count = cJSON_GetArraySize(rows);
    hints->cols_count = cJSON_GetArraySize(cols);

    // Allouer de la mémoire pour les indices de lignes
    hints->rows = (int **)malloc(hints->rows_count * sizeof(int *));
    if (!hints->rows) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        cJSON_Delete(root);
        free(json_content);
        free(hints);
        return NULL;
    }

    // Allouer de la mémoire pour les indices de colonnes
    hints->cols = (int **)malloc(hints->cols_count * sizeof(int *));
    if (!hints->cols) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        cJSON_Delete(root);
        free(json_content);
        free(hints->rows);
        free(hints);
        return NULL;
    }

    // Remplir les indices de lignes
    for (int i = 0; i < hints->rows_count; i++) {
        cJSON *row = cJSON_GetArrayItem(rows, i);
        int count = cJSON_GetArraySize(row);
        hints->rows[i] = (int *)malloc((count + 1) * sizeof(int)); // +1 pour stocker 0 à la fin
        if (!hints->rows[i]) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            cJSON_Delete(root);
            free(json_content);
            // Libérer la mémoire allouée précédemment
            for (int j = 0; j < i; j++) {
                free(hints->rows[j]);
            }
            free(hints->rows);
            free(hints->cols);
            free(hints);
            return NULL;
        }
        for (int j = 0; j < count; j++) {
            hints->rows[i][j] = cJSON_GetArrayItem(row, j)->valueint;
        }
        hints->rows[i][count] = 0; // Terminer avec 0
    }

    // Remplir les indices de colonnes
    for (int i = 0; i < hints->cols_count; i++) {
        cJSON *col = cJSON_GetArrayItem(cols, i);
        int count = cJSON_GetArraySize(col);
        hints->cols[i] = (int *)malloc((count + 1) * sizeof(int)); // +1 pour stocker 0 à la fin
        if (!hints->cols[i]) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            cJSON_Delete(root);
            free(json_content);
            // Libérer la mémoire allouée précédemment
            for (int j = 0; j < hints->rows_count; j++) {
                free(hints->rows[j]);
            }
            free(hints->rows);
            for (int j = 0; j < i; j++) {
                free(hints->cols[j]);
            }
            free(hints->cols);
            free(hints);
            return NULL;
        }
        for (int j = 0; j < count; j++) {
            hints->cols[i][j] = cJSON_GetArrayItem(col, j)->valueint;
        }
        hints->cols[i][count] = 0; // Terminer avec 0
    }

    // Libérer la mémoire utilisée
    cJSON_Delete(root);
    free(json_content);

    return hints;
}

