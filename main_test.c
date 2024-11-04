#include <stdio.h>
#include <limits.h>  // Pour INT_MAX
#include "map.h"
#include "loc.h"
#include "moves.h"
#include "tree.h"

int main() {
    // Charger la carte
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);

    // Afficher les sols
    printf("Soils:\n");
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }

    // Afficher les coûts alignés à gauche sur 5 caractères
    printf("Costs:\n");
    for (int i = 0; i < map.y_max; i++) {
        for (int j = 0; j < map.x_max; j++) {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }

    // Afficher la carte complète
    displayMap(map);

    // Définir les mouvements possibles pour l'arbre
    int moves[] = {10, 20, 30, -10, 1, 2, -1, -2, 0};  // Exemples de mouvements (10m, 20m, etc.)
    int num_moves = sizeof(moves) / sizeof(moves[0]);

    // Créer le nœud racine avec une valeur initiale (ex. 0) et sans mouvement
    Node* root = createNode(0, 0);

    // Construire l'arbre avec une profondeur de 5 pour respecter les critères de 18,730 nœuds
    buildTree(root, 0, 5, moves, num_moves);

    // Compter le nombre de nœuds dans l'arbre
    int totalNodes = countNodes(root);
    printf("Total number of nodes in the tree: %d\n", totalNodes);

    int totalLeaves = countLeaves(root);
    printf("Total number of leaves in the tree: %d\n", totalLeaves);

    // Afficher la structure de l'arbre
    printf("Tree Structure:\n");
    printTree(root, 0);

    // Trouver la feuille avec la valeur minimale
    Node* minLeaf = findMinLeaf(root);
    if (minLeaf != NULL) {
        printf("Leaf with minimum value: %d, Move: %d\n", minLeaf->value, minLeaf->move);

        // Trouver le chemin vers cette feuille
        Node* path[100];  // Supposons un chemin maximum de 100 étapes
        int pathLength = 0;

        // Passer &pathLength pour permettre à findPathToLeaf de le mettre à jour
        if (findPathToLeaf(root, minLeaf, path, &pathLength)) {
            printf("Path to the leaf with minimum value:\n");
            for (int i = 0; i < pathLength; i++) {
                printf("%d", path[i]->value);
                if (i < pathLength - 1) {
                    printf(" -> ");
                }
            }
            printf("\n");
        } else {
            printf("Path to the minimum leaf not found.\n");
        }
    } else {
        printf("No leaf found in the tree.\n");
    }

    // Libérer la mémoire allouée pour l'arbre
    freeTree(root);

    return 0;
}






