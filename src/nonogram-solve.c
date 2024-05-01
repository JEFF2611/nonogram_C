//
// Created by engouan on 30/04/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./cJSON.h"
#include "./nonogram.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s hints.json [--board board.pbm] [--output solved.pbm]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* hints_file = argv[1];
    int** board_file = NULL;
    const char* output_file = NULL;

    // Parse command line arguments
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--board") == 0 && i + 1 < argc) {
            board_file = (int **) argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[i + 1];
            i++;
        }
    }

    // Open the input JSON file
    NonoGramHints* hints = read_json_file(hints_file, board_file);
    if (!hints) {
        fprintf(stderr, "Error reading Nonogram puzzle from file: %s\n", hints_file);
        return EXIT_FAILURE;
    }

    // Solve Nonogram puzzle
    solve_nonogram(hints);

    // Cleanup
    nonogram_hints_destroy(hints);

    return EXIT_SUCCESS;
}