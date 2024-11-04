#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tree.h"

// Fonction pour compter le nombre de feuilles dans l'arbre
int countLeaves(Node* root) {
    if (root == NULL) {
        return 0;
    }

    // Si le nœud n'a pas d'enfants, c'est une feuille
    if (root->num_children == 0) {
        return 1;
    }

    int leaf_count = 0;

    // Parcourir chaque enfant et accumuler le nombre de feuilles
    for (int i = 0; i < root->num_children; i++) {
        leaf_count += countLeaves(root->children[i]);
    }

    return leaf_count;
}



// Fonction pour compter le nombre de nœuds dans l'arbre
int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }

    int count = 1;  // Compter le nœud actuel

    // Compter les nœuds des enfants
    for (int i = 0; i < root->num_children; i++) {
        count += countNodes(root->children[i]);
    }

    return count;
}

// Renvoie le nombre de mouvements possibles pour un niveau donné de l'arbre,
// en fonction de la profondeur, conformément aux contraintes du projet.
int calculateNumMoves(int depth) {
    switch(depth) {
        case 0: return 9; // Niveau 1
        case 1: return 8; // Niveau 2
        case 2: return 7; // Niveau 3
        case 3: return 6; // Niveau 4
        case 4: return 5; // Niveau 5
        default: return 0;
    }
}


// Fonction pour gérer l'affichage de l'indentation en fonction de la profondeur
void printIndentation(int depth, int isLastChild) {
    for (int i = 0; i < depth - 1; i++) {
        printf("|   ");
    }
    if (depth > 0) {
        printf(isLastChild ? "`-- " : "|-- ");
    }
}

// Fonction pour afficher l'arbre avec des branches '|' et des '--'
void printTree(Node* node, int depth) {
    if (node == NULL) return;

    // Si c'est la racine, on l'affiche sans indentation initiale
    if (depth == 0) {
        printf("Root Value: %d, Move: %d\n", node->value, node->move);
    } else {
        // Afficher l'indentation et les symboles de branche en fonction de la profondeur
        printIndentation(depth, node->num_children == 0);
        printf("Node Value: %d, Move: %d\n", node->value, node->move);
    }

    // Récursivement afficher tous les enfants
    for (int i = 0; i < node->num_children; i++) {
        printTree(node->children[i], depth + 1);
    }
}

// Fonction pour créer un nœud
Node* createNode(int value, int move) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->move = move;
    newNode->children = NULL;
    newNode->num_children = 0;
    return newNode;
}

void buildTree(Node* root, int depth, int max_depth, int* moves, int num_moves) {
    if (depth >= max_depth) {
        // Arrêter si la profondeur maximale est atteinte (feuille atteinte)
        root->num_children = 0;
        root->children = NULL;
        return;
    }

    // Calculer le nombre de mouvements possibles pour ce niveau
    int current_num_moves = calculateNumMoves(depth);
    root->num_children = current_num_moves;
    root->children = (Node**)malloc(current_num_moves * sizeof(Node*));  // Allouer de la mémoire pour les enfants

    // Initialiser les enfants avec une valeur de déplacement et coût spécifique
    for (int i = 0; i < current_num_moves; i++) {
        int child_value = root->value + moves[i]; // Calculer la nouvelle valeur en fonction du mouvement
        root->children[i] = createNode(child_value, moves[i]);

        // Générer le tableau des mouvements restants pour le prochain niveau
        int* next_moves = (int*)malloc((current_num_moves - 1) * sizeof(int));
        int index = 0;
        for (int j = 0; j < current_num_moves; j++) {
            if (j != i) {
                next_moves[index++] = moves[j];
            }
        }

        // Appel récursif pour construire les enfants de chaque nœud
        buildTree(root->children[i], depth + 1, max_depth, next_moves, current_num_moves - 1);

        // Libérer la mémoire temporaire utilisée pour les mouvements restants
        free(next_moves);
    }
}



int findPath(Node* root, Node* target, int path[], int pathLength) {
    if (root == NULL) return 0;

    path[pathLength] = root->move;
    pathLength++;

    if (root == target) return 1;

    for (int i = 0; i < root->num_children; i++) {
        if (findPath(root->children[i], target, path, pathLength)) {
            return 1;
        }
    }

    pathLength--;
    return 0;
}

void freeTree(Node* root) {
    if (root == NULL) return;

    for (int i = 0; i < root->num_children; i++) {
        freeTree(root->children[i]);
    }

    free(root->children);
    free(root);
}



Node* findMinLeaf(Node* root) {
    Node* minLeaf = NULL;
    int minValue = INT_MAX;
    findMinLeafHelper(root, &minLeaf, &minValue);
    return minLeaf;
}

// Fonction auxiliaire récursive pour trouver la feuille de valeur minimale
void findMinLeafHelper(Node* node, Node** minLeaf, int* minValue) {
    if (node == NULL) return;

    // Considérer le nœud comme une feuille seulement s'il n'a pas d'enfants
    if (node->num_children == 0) {
        //printf("Found leaf: Value = %d, Move = %d\n", node->value, node->move);  // Message de débogage
        if (node->value < *minValue) {
            *minValue = node->value;
            *minLeaf = node;
            //printf("Updated minLeaf to Value = %d, Move = %d\n", node->value, node->move);  // Message de débogage
        }
    } else {
        // Appel récursif pour chaque enfant
        for (int i = 0; i < node->num_children; i++) {
            findMinLeafHelper(node->children[i], minLeaf, minValue);
        }
    }
}



// Fonction pour trouver le chemin de la racine vers une feuille spécifique

int findPathToLeaf(Node* root, Node* target, Node* path[], int* pathLength) {
    if (root == NULL) return 0;

    // Ajouter le nœud actuel au chemin
    path[*pathLength] = root;
    (*pathLength)++;

    // Si le nœud courant est le nœud cible, on a trouvé le chemin
    if (root == target) return 1;

    // Parcourir chaque enfant
    for (int i = 0; i < root->num_children; i++) {
        if (findPathToLeaf(root->children[i], target, path, pathLength)) {
            return 1;  // Chemin trouvé
        }
    }

    // Si le chemin n'a pas été trouvé, revenir en arrière
    (*pathLength)--;
    return 0;
}

