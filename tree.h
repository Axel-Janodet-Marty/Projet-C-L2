//
// Created by Skander on 23/10/2024.
//

#ifndef UNTITLED1_TREE_H
#define UNTITLED1_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "map.h"
#include "moves.h"




typedef struct s_node
{
    int value; // cout de la case actuelle
    int depth; // Profondeur des racines de l'aarbre
    t_move move; // Mouvement fait pour arriver à cette case (à été utilisée pour la recostitution des mouvements )
    struct s_node *parent; // Node d'avant (à été utilisée pour retrouver le chemin min)
    struct s_node **sons; // Node apres
    int nbSons; // Qui veut aussi dire la taille de l'arbre
} t_node;

typedef struct s_node t_node, *p_node;


t_node* find_best_min(t_node* );

void print_moves(t_move* , int );

t_move* get_moves_from_start(t_node* , int* );

void explore(t_node* , t_node** , int* , int* );

p_node Init_Node(int , int , t_move , t_localisation , t_map , p_node );

void printTree(t_node *, int *, int );

p_node BuildTree(int , int , t_move , t_move* , t_localisation , t_map , p_node );

int search_min(t_node *);







#endif //UNTITLED1_TREE_H

