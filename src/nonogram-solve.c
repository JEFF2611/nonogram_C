//
// Created by engouan on 30/04/2024.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#include "./cJSON.h"
#include "./nonogram.h"
#include "./nonogram.inc"
#include "./pnmio.h" //PBM file, read and write

#define MAX_HINTS 32


bool fill_spaces_between_indices(NonoGramHints *hints, int **board, int index, int size) {
    bool progress = false;
    int *indices = (index < hints->rows_count) ? hints->rows[index] : hints->cols[index];

    if (index > 0) { // Vérifie si ce n'est pas la première ligne
        int *prevRow = (index < hints->rows_count) ? hints->rows[index - 1] : hints->cols[index - 1];
        for (int i = 0; indices[i] != 0; i++) {
            int count = indices[i];
            int start = 0;

            // Trouver le début de la séquence
            while (start < size && board[index][start] == 0) {
                start++;
            }

            // Remplir les cases vides entre les indices en vérifiant la case précédente
            for (int j = start + count; j < size && j < start + count + 1; j++) {
                if (board[index][j] == 0 && (j == 0 || board[index - 1][j - 1] == 1)) {
                    board[index][j] = 1;
                    progress = true;
                }
            }
        }
    }

    return progress;
}

bool fill_cells_between_indices(NonoGramHints *hints, int **board, int index, int size) {
    bool progress = false;
    int *indices = (index < hints->rows_count) ? hints->rows[index] : hints->cols[index];

    if (index > 0) { // Vérifie si ce n'est pas la première ligne
        int *prevRow = (index < hints->rows_count) ? hints->rows[index - 1] : hints->cols[index - 1];
        for (int i = 0; indices[i] != 0; i++) {
            int count = indices[i];
            int start = 0;

            // Trouver le début de la séquence
            while (start < size && board[index][start] == 1) {
                start++;
            }

            // Remplir les cases pleines entre les indices en vérifiant la case précédente
            for (int j = start + count; j < size && j < start + count + 1; j++) {
                if (board[index][j] == 1 && (j == 0 || board[index - 1][j - 1] == 0)) {
                    board[index][j] = 0;
                    progress = true;
                }
            }
        }
    }

    return progress;
}


// Fonction pour appliquer les stratégies de résolution sur une ligne
bool apply_row_strategies(NonoGramHints *hints, int **board, int row, int cols_count) {
    bool progress = false;

    // Stratégie 1 : Remplir les cases vides entre les indices de ligne
    progress |= fill_spaces_between_indices(hints, board, row, cols_count);

    // Stratégie 2 : Remplir les cases pleines entre les indices de ligne
    progress |= fill_cells_between_indices(hints, board, row, cols_count);

    return progress;
}

// Fonction pour appliquer les stratégies de résolution sur une colonne
bool apply_col_strategies(NonoGramHints *hints, int **board, int col, int rows_count) {
    bool progress = false;

    // Stratégie 1 : Remplir les cases vides entre les indices de colonne
    progress |= fill_spaces_between_indices(hints, board, col, rows_count);

    // Stratégie 2 : Remplir les cases pleines entre les indices de colonne
    progress |= fill_cells_between_indices(hints, board, col, rows_count);

    return progress;
}


// Fonction pour appliquer les stratégies de résolution
bool apply_strategies(NonoGramHints *hints, int **board, int rows_count, int cols_count) {
    bool progress = false;

    // Parcourir les lignes
    for (int row = 0; row < rows_count; row++) {
        progress |= apply_row_strategies(hints, board, row, cols_count);
    }

    // Parcourir les colonnes
    for (int col = 0; col < cols_count; col++) {
        progress |= apply_col_strategies(hints, board, col, rows_count);
    }

    return progress;
}

// Fonction pour résoudre le Nonogram
void solve_nonogram(NonoGramHints *hints, int **board) {
    int rows_count = nonogram_hints_get_rows_count(hints);
    int cols_count = nonogram_hints_get_cols_count(hints);
    bool progress = true;

    while (progress) {
        progress = false;

        // Appliquer les stratégies de résolution
        progress |= apply_strategies(hints, board, rows_count, cols_count);
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

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hints.json [--board board.pbm] [--output solved.pbm]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* hints_file = argv[1];
    const char* board_file = NULL;
    int** board_data = NULL;
    const char* output_file = NULL;

    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--board") == 0 && i + 1 < argc) {
            board_file = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        }
    }
    // Charger les indices à partir du fichier JSON
    NonoGramHints *hints = parse_json(hints_file);
    if (!hints) {
        fprintf(stderr, "Error: Unable to parse JSON file\n");
        return EXIT_FAILURE;
    }

    // Crée le board en fonction des tailles de lignes et colonnes du Nonogram
    int rows_count = nonogram_hints_get_rows_count(hints);
    int cols_count = nonogram_hints_get_cols_count(hints);
    int **board = (int **)malloc(rows_count * sizeof(int *));
    if (!board) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        nonogram_hints_destroy(hints);
        return EXIT_FAILURE;
    }
    for (int i = 0; i < rows_count; i++) {
        board[i] = (int *)calloc(cols_count, sizeof(int));
        if (!board[i]) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            nonogram_hints_destroy(hints);
            return EXIT_FAILURE;
        }
    }

    // Solve le Nonogram
    solve_nonogram(hints, board);


    // Écrivez le plateau de jeu dans un fichier PBM
    nonogram_write_pbm(output_file, board, hints->rows_count, hints->cols_count);

    printf("%s\n", nonogram_hints_to_string(hints));
    printf("OK\n");

    // Libérer la mémoire utilisée
    free(hints);
    for (int i = 0; i < hints->rows_count; i++) {
        free(board[i]);
    }
    free(board);

    return EXIT_SUCCESS;
}