#include <stdio.h>
#include <limits.h>  // Pour INT_MAX
#include "map.h"
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

    displayMap(map);

    // Définir les mouvements possibles pour l'arbre
    int moves[] = {10, 20, 30, -10, 1, 2, -1, -2, 0};
    int num_moves = sizeof(moves) / sizeof(moves[0]);

    // Créer le nœud racine avec une valeur initiale (par ex. 0) et sans mouvement
    Node* root = createNode(0, 0, num_moves);

    // Construire l'arbre avec une profondeur de 3 et un nombre maximum de mouvements à chaque niveau
    buildTree(root, 0, 3, moves, num_moves);

    // Afficher la structure de l'arbre
    printf("Tree Structure:\n");
    printTree(root, 0);

    // Trouver la feuille avec la valeur minimale en utilisant findMinLeaf avec l'assistant findMinLeafHelper
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
                // Afficher chaque nœud dans le format "xN", où N est la valeur du nœud
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


/*#include <stdio.h>
#include <limits.h>  // Pour INT_MAX
#include "tree.h"

int main() {
    // Création manuelle de l'arbre pour le test
    Node* root = createNode(10, 0, 2);         // Racine avec valeur 10, 2 enfants
    root->children[0] = createNode(5, 1, 2);   // Enfant gauche de la racine avec valeur 5
    root->children[1] = createNode(20, 2, 0);  // Enfant droit de la racine avec valeur 20 (feuille)

    // Ajout d'enfants à l'enfant gauche
    root->children[0]->children[0] = createNode(2, 3, 0);  // Feuille avec valeur 2
    root->children[0]->children[1] = createNode(8, 4, 0);  // Feuille avec valeur 8

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
}*/
