#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "tree.h"
#include <time.h>
#include <windows.h>


int main() {
    SetConsoleOutputCP(CP_UTF8);
    clock_t start, end;
    double cpu_time_used;

    // Phase 1 : Création de la carte
    start = clock();
    t_map map = createMapFromFile("..\\maps\\example1.map");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    printf("Temps de création de la carte : %.6f secondes\n", cpu_time_used);

    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }

    // Phase 2 : Génération des mouvements aléatoires
    start = clock();
    t_move* test = getRandomMoves(NB_tab_moves);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Temps de génération des mouvements aléatoires : %.6f secondes\n", cpu_time_used);

    for (int j = 0; j < NB_tab_moves; j++) {
        printf("%s   ", getMoveAsString(test[j]));
    }
    printf("\n");

    // Phase 3 : Initialisation de la localisation
    t_localisation loc = loc_init(0, 0, NORTH);
    printf("Position initiale (%d:%d), cout de la case initiale :  %d\n", loc.pos.y, loc.pos.x, map.costs[loc.pos.y][loc.pos.x]);

    // Phase 4 : Construction de l'arbre
    start = clock();
    t_node* node = BuildTree(NB_tab_moves, 0, INITIAL_POS, test, loc, map, NULL);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Temps de construction de l'arbre : %.6f secondes\n", cpu_time_used);

    // Phase 5 : Recherche de la feuille de valeur minimale
    start = clock();
    int min_value = search_min(node);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Min = %d\n", min_value);
    printf("Temps de recherche de la valeur minimale : %.6f secondes\n", cpu_time_used);

    // Phase 6 : Affichage de l'arbre


    t_node* best_min_node = find_best_min(node); // Trouver le meilleur minimum
    if (best_min_node != NULL) {
        int move_count;
        t_move* moves = get_moves_from_start(best_min_node, &move_count);

        printf("Liste des mouvements effectués par MARC :\n");
        print_moves(moves, move_count);

        // Libérer la mémoire allouée pour les mouvements
        free(moves);
    } else {
        printf("L'arbre est vide ou aucun meilleur minimum trouvé.\n");
    }

    printf("\nArbre des déplacements :\n");
    int architecture[100] = {0}; // Taille maximale pour la profondeur de l'arbre à print
    int eye;
    printf("Es ce qu'il y a un truc a check ?");
    scanf("%d",&eye);

    if (eye == 1) {
        printTree(node, architecture, 0);
    }

    // Nettoyage
    free(node);
    free(test);

    return 0;
}
