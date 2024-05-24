    //
    // Created by engouan,wbernard,jb--guillon,kdamasceno,akerraf,qvievard on 30/03/2024.
    //

    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include "./cJSON.h"
    #include "./nonogram.h"
    #include "./nonogram.inc"
    #include "./pnmio.h" //PBM file, read and write

    /**
     * @brief Maximum number of hints
     */
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
                // Remplir les blocs
                for (int j = 0; j < hint; j++) {
                    if (col < cols_count) {
                        board[row][col] = 1;
                        col++;
                    }
                }
                // Ajouter un espace entre les blocs si c'est permis
                if (col < cols_count && i < MAX_HINTS - 1 && hints->rows[row][i + 1] != 0) {
                    col++;
                }
            }
        }

        return board;
    }

    // Fonction pour créer des hints
    /**
     * @brief Create hints from a 2D array
     * @param hints
     * @param count
     * @return A 2D array of hints
     */
    int **create_hints(int hints[][MAX_HINTS], int count) {
        int **array = (int **)malloc(count * sizeof(int *));
        for (int i = 0; i < count; i++) {
            array[i] = (int *)malloc(MAX_HINTS * sizeof(int));
            for (int j = 0; j < MAX_HINTS; j++) {
                array[i][j] = hints[i][j];
            }
        }
        return array;
    }

    // Fonction pour libérer la mémoire allouée au tableau
    /**
     * @brief Free a nonogram board
     * @param board The board
     * @param rows_count The number of rows in the board
     */
    void free_nonogram_board(int **board, int rows_count) {
        for (int i = 0; i < rows_count; i++) {
            free(board[i]);
        }
        free(board);
    }

    // Fonction de test
    /**
     * @brief Print a board
     * @param board The board
     * @param rows_count The number of rows in the board
     * @param cols_count The number of columns in the board
     */
    void print_board(int **board, int rows_count, int cols_count) {
        for (int i = 0; i < rows_count; i++) {
            for (int j = 0; j < cols_count; j++) {
                printf("%d ", board[i][j]);
            }
            printf("\n");
        }
    }
    /**
     * @brief Parse a JSON file containing nonogram hints
     * @param filename The JSON file to parse
     * @return A nonogram hints object
     */
    NonoGramHints *parse_json(const char *filename) {
        // Open the JSON file
        FILE *file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "Error: Unable to open file %s\n", filename);
            return NULL;
        }

        // Read the content of the file
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *json_content = (char *)malloc(file_size + 1);
        if (!json_content) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            return NULL;
        }

        fread(json_content, 1, file_size, file);
        fclose(file);

        // Add null termination at the end of the JSON string
        json_content[file_size] = '\0';

        // Parse the JSON content
        cJSON *root = cJSON_Parse(json_content);
        if (!root) {
            fprintf(stderr, "Error: Failed to parse JSON\n");
            free(json_content);
            return NULL;
        }

        // Extract row and column hints
        cJSON *rows = cJSON_GetObjectItem(root, "rows");
        cJSON *cols = cJSON_GetObjectItem(root, "cols");
        if (!rows || !cols) {
            fprintf(stderr, "Error: Invalid JSON format\n");
            cJSON_Delete(root);
            free(json_content);
            return NULL;
        }

        // Create a NonoGramHints object
        NonoGramHints *hints = (NonoGramHints *)malloc(sizeof(NonoGramHints));
        if (!hints) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            cJSON_Delete(root);
            free(json_content);
            return NULL;
        }

        hints->rows_count = cJSON_GetArraySize(rows);
        hints->cols_count = cJSON_GetArraySize(cols);

        // Allocate memory for row hints
        hints->rows = (int **)malloc(hints->rows_count * sizeof(int *));
        if (!hints->rows) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            cJSON_Delete(root);
            free(json_content);
            free(hints);
            return NULL;
        }

        // Allocate memory for column hints
        hints->cols = (int **)malloc(hints->cols_count * sizeof(int *));
        if (!hints->cols) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            cJSON_Delete(root);
            free(json_content);
            free(hints->rows);
            free(hints);
            return NULL;
        }

        // Fill row hints
        for (int i = 0; i < hints->rows_count; i++) {
            cJSON *row = cJSON_GetArrayItem(rows, i);
            int count = cJSON_GetArraySize(row);
            hints->rows[i] = (int *)malloc((count + 1) * sizeof(int)); // +1 to store 0 at the end
            if (!hints->rows[i]) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                cJSON_Delete(root);
                free(json_content);
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
            hints->rows[i][count] = 0; // Terminate with 0
        }

        // Fill column hints
        for (int i = 0; i < hints->cols_count; i++) {
            cJSON *col = cJSON_GetArrayItem(cols, i);
            int count = cJSON_GetArraySize(col);
            hints->cols[i] = (int *)malloc((count + 1) * sizeof(int)); // +1 to store 0 at the end
            if (!hints->cols[i]) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                cJSON_Delete(root);
                free(json_content);
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
            hints->cols[i][count] = 0; // Terminate with 0
        }

        // Free used memory
        cJSON_Delete(root);
        free(json_content);

        return hints;
    }

    /**
     * @brief Main function
     * @param argc
     * @param argv
     * @return
     */
    int main(int argc, char *argv[]) {
        if (argc < 2) {
            fprintf(stderr, "Usage: %s hints.json [--output solved.pbm]\n", argv[0]);
            return EXIT_FAILURE;
        }

        const char *hints_file = argv[1];
        const char *output_file = NULL;

        // Parse command line arguments
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
                output_file = argv[i + 1];
                i++;
            }
        }
        // Load hints from the JSON file
        NonoGramHints *hints = parse_json(hints_file);
        if (!hints) {
            return EXIT_FAILURE;
        }

        int **board = nonogram_board_create_from_hints(hints);
        if (board) {
            print_board(board, hints->rows_count, hints->cols_count);
            free_nonogram_board(board, hints->rows_count);
        }

        // Libérer la mémoire allouée pour les hints
        for (int i = 0; i < hints->rows_count; i++) {
            free(hints->rows[i]);
        }
        free(hints->rows);

        for (int i = 0; i < hints->cols_count; i++) {
            free(hints->cols[i]);
        }
        free(hints->cols);

        free(hints);
        return 0;
    }