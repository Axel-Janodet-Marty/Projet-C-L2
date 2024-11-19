//
// Created by Skander on 23/10/2024.
//

#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

typedef struct Node {
    int value;              // Valeur de la case atteinte après un mouvement
    int move;               // Mouvement associé
    struct Node** children; // Enfants du nœud
    int num_children;       // Nombre d'enfants
} Node;


int countLeaves(Node* root);
int countNodes(Node* root);
int calculateNumMoves(int depth);
t_move* generate_random_moves();
//void printTree(t_node *node, int *architecture, int depth);
// Fonction pour créer un nouveau nœud
Node* createNode(int value, int move);

// Fonction pour construire l'arbre
void buildTree(Node* root, int depth, int max_depth, int* moves, int num_moves);

// Fonction pour trouver la feuille avec la valeur minimale
Node* findMinLeaf(Node* root);

// Fonction pour trouver le chemin de la racine vers une feuille spécifique
int findPath(Node* root, Node* target, int path[], int pathLength);

// Fonction pour libérer la mémoire de l'arbre
void freeTree(Node* root);

// Fonction pour afficher l'arbre avec indentation et des branches
void printTree(Node* node, int depth);


// Fonction auxiliaire pour trouver la feuille avec la valeur minimale
void findMinLeafHelper(Node* node, Node** minLeaf, int* minValue);

// trouve le chemin associé a la feuille avec la valeur minimal
int findPathToLeaf(Node* root, Node* target, Node* path[], int* pathLength);

#endif  // UNTITLED1_TREE_H
