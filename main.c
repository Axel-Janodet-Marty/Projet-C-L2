#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "map.h"
#include "tree.h"
#include "stdbool.h"

// Fonctions auxiliaires
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
    printf("9. Étude de complexité (profondeur et choix)\n");
    printf("10. Quitter\n");
    printf("Votre choix : ");
}

int main() {
    int NB_tab_moves = 9;
    int NB_choices = 5;
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
        char filename[256], mapPath[256];
        printf("Entrez le nom de la carte (sans .map) : ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';
        snprintf(mapPath, sizeof(mapPath), "..\\maps\\%s.map", filename);
        map = createMapFromFile(mapPath);
    } else if (choice == 2) {
        int xdim, ydim;
        printf("Entrez les dimensions de la carte (xdim) : ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &xdim);

        printf("Entrez les dimensions de la carte (ydim) : ");
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &ydim);

        map = generateRandomMap(xdim, ydim);
    } else {
        printf("Choix invalide.\n");
        return 1;
    }

    printf("\nCarte créée avec dimensions %d x %d\n", map.y_max, map.x_max);

    int exit = 0;
    bool activeLoc = FALSE;
    t_move *test = NULL;
    t_localisation loc;
    t_node *node = NULL;

    while (!exit) {
        afficherMenu();
        fgets(buffer, sizeof(buffer), stdin);
        sscanf(buffer, "%d", &choice);

        switch (choice) {
            case 1: // Afficher la carte
                printf("Sols :\n");
                displayMap(map);
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
                loc = inputLocalisation(map);
                activeLoc = TRUE;
                printf("Coût de la case initiale : %d\n", map.costs[loc.pos.y][loc.pos.x]);
                break;

            case 4: // Construire l'arbre
                if (!test || !activeLoc) {
                    printf("Générez des mouvements (2) et initialisez la localisation (3) d'abord.\n");
                    break;
                }
                start = clock();
                node = BuildTree(NB_choices,NB_tab_moves, 0, INITIAL_POS, test, loc, map, NULL);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Temps de construction de l'arbre : %.3f sec\n", cpu_time_used);
                break;

            case 5: // Rechercher la feuille de valeur minimale
                if (!node) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                start = clock();
                int min_value = search_min(node);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Valeur minimale : %d\n", min_value);
                printf("--> Temps de recherche : %.3f sec\n", cpu_time_used);
                break;

            case 6: // Calculer le chemin vers la feuille minimale
                if (!node) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                start = clock();
                t_node *best_min_node = find_best_min(node);
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("--> Temps de calcul du chemin : %.3f sec\n", cpu_time_used);
                if (best_min_node) {
                    int move_count;
                    t_move *moves = get_moves_from_start(best_min_node, &move_count);
                    printf("--> Mouvements :\n");
                    print_moves(moves, move_count);
                    free(moves);
                } else {
                    printf("--> Aucun chemin trouvé.\n");
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
                if (!node) {
                    printf("Construisez d'abord l'arbre (option 4).\n");
                    break;
                }
                printf("--> Affichage de l'arbre :\n");
                int architecture[100] = {0};
                printTree(node, architecture, 0);
                break;

            case 9: // Étude de complexité
                if (!test || !activeLoc) {
                    printf("Générez des mouvements (2) et initialisez la localisation (3) d'abord.\n");
                    break;
                }
                printf("--> Étude de complexité :\n");
                for (int Nb_moves = 3; Nb_moves <= NB_tab_moves; Nb_moves++) {
                    for (int choices = 3; choices <= NB_choices; choices++) {
                        printf("\nNb_Tirages : %d, N_Choix : %d\n", Nb_moves, choices);
                        start = clock();
                        t_node *test_tree = BuildTree(choices,Nb_moves, 0, INITIAL_POS, test, loc, map, NULL);
                        end = clock();
                        printf("--> Temps de construction : %.3f sec\n", ((double)(end - start)) / CLOCKS_PER_SEC);
                        free(test_tree); // Nettoyer l'arbre
                    }
                }
                break;

            case 10: // Quitter
                exit = 1;
                printf("Fin du programme.\n");
                break;

            default:
                printf("Option invalide.\n");
                break;
        }
    }

    // Nettoyage final
    if (node) free(node);
    if (test) free(test);

    return 0;
}
