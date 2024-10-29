#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tree.h"

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

Node* createNode(int value, int move, int num_children) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;  // Valeur assignée au nœud
    newNode->move = move;
    newNode->num_children = num_children;

    if (num_children > 0) {
        newNode->children = (Node**)malloc(num_children * sizeof(Node*));
        for (int i = 0; i < num_children; i++) {
            newNode->children[i] = NULL;
        }
    } else {
        newNode->children = NULL;
    }

    return newNode;
}

// Fonction pour construire l'arbre avec des valeurs réinitialisées à 0 à chaque niveau
void buildTree(Node* root, int depth, int max_depth, int* moves, int num_moves) {
    if (depth >= max_depth) {
        // Ne pas ajouter d'enfants si la profondeur maximale est atteinte
        root->num_children = 0;
        root->children = NULL;
        return;
    }

    // Définir le nombre d'enfants pour les nœuds qui ne sont pas des feuilles
    root->num_children = num_moves;
    root->children = (Node**)malloc(num_moves * sizeof(Node*));  // Allouer de la mémoire pour les enfants

    // Initialiser les enfants
    int childValue = 0;
    for (int i = 0; i < num_moves; i++) {
        // Créer chaque enfant avec une valeur et un mouvement spécifique
        root->children[i] = createNode(childValue, moves[i], 0);  // Num_children de l'enfant sera mis à jour plus tard
        childValue++;  // Incrémenter la valeur courante pour chaque enfant
        // Construire l'arbre récursivement pour chaque enfant
        buildTree(root->children[i], depth + 1, max_depth, moves, num_moves - 1);
    }
}


/*Node* findMinLeaf(Node* root) {
    if (root == NULL) return NULL;
    if (root->num_children == 0) return root;

    Node* minLeaf = NULL;
    for (int i = 0; i < root->num_children; i++) {
        Node* candidate = findMinLeaf(root->children[i]);
        if (minLeaf == NULL || (candidate != NULL && candidate->value < minLeaf->value)) {
            minLeaf = candidate;
        }
    }
    return minLeaf;
}*/

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

