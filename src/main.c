#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h" // Bibliothèque pour lire le fichier JSON
#include "libpnmio.h" // Bibliothèque pour lire le fichier PBM

int main(int argc, char **argv) {
	if (argc < 2 || argc > 6) {
        fprintf(stderr, "Usage: %s hints.json [--board board.pbm] [--output solved.pbm]\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* hints_file = argv[1];
    const char* board_file = NULL;
    const char* output_file = NULL;

    // Analysez les arguments de la ligne de commande
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--board") == 0 && i + 1 < argc) {
            board_file = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            fprintf(stderr, "Unknown option or missing argument: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Appelez la fonction pour résoudre le Nonogramme
    solve_nonogram(hints_file, board_file, output_file);

    return EXIT_SUCCESS;
}