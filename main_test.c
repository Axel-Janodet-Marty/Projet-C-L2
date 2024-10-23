#include <stdio.h>
#include "map.h"
#include "tree.h"

int main() {
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);

    // Afficher les sols
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    // Afficher les coûts alignés à gauche sur 5 caractères
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }

    displayMap(map);

    int moves[] = {10, 20, 30, -10, 1, 2, -1, -2, 0};  // 9 mouvements possibles
    int num_moves = sizeof(moves) / sizeof(moves[0]);

    // Créer le nœud racine avec une valeur initiale (par ex. 0) et sans mouvement
    Node* root = createNode(0, 0, num_moves);

    // Construire l'arbre avec une profondeur de 3 et un nombre maximum de mouvements à chaque niveau
    buildTree(root, 0, 3, moves, num_moves);

    // Afficher la structure de l'arbre
    printf("Tree Structure:\n");
    printTree(root, 0);

    // Trouver la feuille avec la valeur minimale
    Node* minLeaf = findMinLeaf(root);
    if (minLeaf != NULL) {
        printf("Leaf with minimum value: %d, Move: %d\n", minLeaf->value, minLeaf->move);
    }

    // Libérer la mémoire allouée pour l'arbre
    freeTree(root);

    return 0;
}
