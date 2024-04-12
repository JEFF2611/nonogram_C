* https://github.com/nkkav/libpnmio (fichier optionnel à lire au format ASCII PBM)
* https://github.com/DaveGamble/cJSON (fichier au format JSON)
* https://github.com/chdemko/c-arithmetic (Inspo pour les tests)
* https://google.github.io/styleguide/cppguide.html#Formatting (Style d'indentation)
* https://github.com/ollix/cclint
* https://clang.llvm.org/docs/ClangFormat.html
  
## Documentation
* https://www.sphinx-doc.org/en/master/
* https://www.doxygen.nl/

## Ordre des inclusions
1. Inclusion du fichier directement lié au fichier .c qui l’inclut suivi d’une ligne vide ;
2. inclusion des fichiers d’en-tête du C standard suivis d’une ligne vide ;
3. inclusion des fichiers d’en-tête provenant d’autres librairies suivis d’une ligne vide ;
4. inclusion des fichiers d’en-tête du projet suivi d’une ligne vide ;
5. inclusion des fichiers d’inclusion (extension .inc)