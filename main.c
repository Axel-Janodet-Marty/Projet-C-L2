#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "tree.h"
#include <time.h>
#include <windows.h>
#include "stdbool.h"

void afficherMenu() {
    printf("\n--- MENU ---\n");
    printf("1. Afficher la carte (sols et coûts)\n");
    printf("2. Générer des mouvements aléatoires\n");
    printf("3. Initialiser la localisation\n");
    printf("4. Construire l'arbre\n");
    printf("5. Rechercher la feuille de valeur minimale\n");
    printf("6. Calculer le chemin vers la feuille minimale\n");
    printf("7. Rapport sur la situation\n");
    printf("8. Afficher l'arbre entier\n");
    printf("9. Passer en interface graphique\n");
    printf("10. Quitter\n");
    printf("Votre choix : ");
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    clock_t start, end;
    double cpu_time_used;

    t_map map;
    char buffer[256];
    int choice;

    // Choix de la méthode de création de la carte
    printf("Choisissez une méthode pour créer la carte :\n");
    printf("1. Charger une carte depuis un fichier\n");
    printf("2. Générer une carte aléatoire\n");
    printf("Votre choix : ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &choice);

    if (choice == 1) {
        char filename[256];
        printf("Entrez le chemin du fichier de la carte : ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';
        map = createMapFromFile(filename);
    } else if (choice == 2) {
        int xdim, ydim;
        printf("Entrez les dimensions de la carte (xdim) : ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &xdim) != 1 || xdim <= 0) {
            fprintf(stderr, "Erreur : dimension invalide pour xdim\n");
            return 1;
        }

        printf("Entrez les dimensions de la carte (ydim) : ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%d", &ydim) != 1 || ydim <= 0) {
            fprintf(stderr, "Erreur : dimension invalide pour ydim\n");
            return 1;
        }

        map = generateRandomMap(xdim, ydim);
    } else {
        printf("Choix invalide.\n");
        return 1;
    }

    printf("Carte créée avec dimensions %d x %d\n", map.y_max, map.x_max);

    int exit = 0;
    bool ActiveLoc = FALSE;
    t_move *test = NULL;
    t_localisation loc;
    t_node *node = NULL;
    int min_value = 0;
    t_node *best_min_node = NULL;

    while (!exit) {
        afficherMenu();
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1: // Afficher la carte
                printf("Sols :\n");
                for (int i = 0; i < map.y_max; i++) {
                    for (int j = 0; j < map.x_max; j++) {
                        printf("%d ", map.soils[i][j]);
                    }
                    printf("\n");
                }
                printf("Coûts :\n");
                for (int i = 0; i < map.y_max; i++) {
                    for (int j = 0; j < map.x_max; j++) {
                        printf("%-5d ", map.costs[i][j]);
                    }
                    printf("\n");
                }
                break;

            case 2: // Générer des mouvements aléatoires
                test = getRandomMoves(NB_tab_moves);
                printf("[   ");
                for (int j = 0; j < NB_tab_moves; j++) {
                    printf("%s   ", getMoveAsString(test[j]));
                }
                printf("]\n");
                break;

            case 3: // Initialiser la localisation
                loc = loc_init(0, 0, NORTH);
                ActiveLoc = TRUE;
                printf("Position initiale (%d:%d), coût de la case initiale : %d\n", loc.pos.y, loc.pos.x, map.costs[loc.pos.y][loc.pos.x]);
                break;

            case 4: // Construire l'arbre
                if (test == NULL) {
                    printf("Générez d'abord les mouvements aléatoires (option 2).\n");
                    break;
                }
                if (ActiveLoc == FALSE) {
                    printf("Initialisez d'abord la localisation (option 3).\n");
                    break;
                }
                start = clock();
                node = BuildTree(NB_tab_moves, 0, INITIAL_POS, test, loc, map, NULL);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Temps de construction de l'arbre : %.3f sec\n", cpu_time_used);
                break;

            case 5: // Rechercher la feuille de valeur minimale
                if (node == NULL) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                start = clock();
                min_value = search_min(node);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Valeur minimale des nodes = %d\n", min_value);
                printf("--> Temps de recherche de la valeur minimale : %.3f sec\n", cpu_time_used);
                break;

            case 6: // Calculer le chemin vers la feuille minimale
                if (node == NULL) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                start = clock();
                best_min_node = find_best_min(node);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Temps de calcul du meilleur chemin : %.3f sec\n", cpu_time_used);

                if (best_min_node != NULL) {
                    int move_count;
                    t_move *moves = get_moves_from_start(best_min_node, &move_count);

                    printf("--> Liste des mouvements effectués par MARC :\n");
                    print_moves(moves, move_count);

                    free(moves);
                } else {
                    printf("--> Aucun meilleur minimum trouvé.\n");
                }
                break;

            case 7: // Rapport sur la situation
                if (node == NULL) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                start = clock();
                checkWin(min_value);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Temps de rapport sur la situation : %.3f sec\n", cpu_time_used);
                break;

            case 8: // Afficher l'arbre entier
                if (node == NULL) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                printf("--> Affichage de l'arbre entier :\n");
                int architecture[100] = {0}; // Tableau d'indentation, taille adaptée à la profondeur maximale
                printTree(node, architecture, 0); // Appel de la fonction avec profondeur initiale 0
                break;

            case 9: // Passer en interface graphique
                printf("--> Interface graphique (non implémentée pour le moment).\n");
                break;

            case 10: // Quitter
                exit = 1;
                printf("Fin du programme.\n");
                break;

            default:
                printf("Option invalide. Réessayez.\n");
                break;
        }
    }

    // Nettoyage
    if (node) free(node);
    if (test) free(test);

    return 0;
}
