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
    t_soil soil; // Type de sol
} t_node;

typedef struct s_node t_node, *p_node;


/**
 * @brief Trouve le nœud correspondant au "meilleur minimum" dans un arbre.
 * @param t_node* root : Pointeur vers la racine de l'arbre.
 * @return t_node* : Pointeur vers le nœud ayant la valeur minimale et la plus faible profondeur.
 */
t_node* find_best_min(t_node* root);


/**
 * @brief Affiche les mouvements effectués sous forme de texte.
 * @param t_move* moves : Tableau contenant les mouvements.
 * @param int move_count : Nombre total de mouvements dans le tableau.
 */
void print_moves(t_move* moves, int move_count);


/**
 * @brief Récupère tous les mouvements effectués depuis la racine jusqu'au nœud donné.
 * @param t_node* node : Pointeur vers le nœud cible.
 * @param int* move_count : Pointeur pour stocker le nombre total de mouvements.
 * @return t_move* : Tableau contenant les mouvements depuis la racine.
 */
t_move* get_moves_from_start(t_node* node, int* move_count);


/**
 * @brief Explore récursivement l'arbre pour trouver le meilleur nœud minimum.
 * @param t_node* node : Nœud actuel en cours d'exploration.
 * @param t_node** best_min : Pointeur vers le meilleur nœud minimum trouvé.
 * @param int* min_value : Pointeur vers la valeur minimale actuelle.
 * @param int* min_depth : Pointeur vers la profondeur minimale actuelle.
 */
void explore(t_node* node, t_node** best_min, int* min_value, int* min_depth);


/**
 * @brief Initialise un nœud dans l'arbre.
 * @param int nb_sons : Nombre de fils du nœud.
 * @param int depth : Profondeur du nœud dans l'arbre.
 * @param t_move move : Mouvement utilisé pour arriver à ce nœud.
 * @param t_localisation loc : Localisation actuelle associée à ce nœud.
 * @param t_map map : Carte associée au nœud.
 * @param p_node parent : Pointeur vers le parent du nœud.
 * @return p_node : Pointeur vers le nœud initialisé.
 */
p_node Init_Node(int nb_sons, int depth, t_move move, t_localisation loc, t_map map, p_node parent);


/**
 * @brief Affiche la structure d'un arbre de manière hiérarchique.
 * @param t_node* node : Pointeur vers la racine de l'arbre ou sous-arbre à afficher.
 * @param int* architecture : Tableau pour suivre l'indentation lors de l'affichage.
 * @param int depth : Profondeur actuelle dans l'arbre.
 */
void printTree(t_node* node, int* architecture, int depth);


/**
 * @brief Construit un arbre de déplacements possibles à partir d'une localisation donnée.
 * @param int NB_Moves : Nombre de mouvements possibles à chaque niveau.
 * @param int depth : Profondeur actuelle dans l'arbre.
 * @param t_move move : Mouvement effectué pour arriver au nœud courant.
 * @param t_move* tab_moves : Tableau des mouvements possibles.
 * @param t_localisation loc : Localisation actuelle du rover.
 * @param t_map map : Carte contenant les informations sur les sols et coûts.
 * @param p_node parent : Pointeur vers le parent du nœud courant.
 * @return p_node : Pointeur vers la racine de l'arbre construit.
 */
p_node BuildTree(int NB_Moves, int depth, t_move move, t_move* tab_moves, t_localisation loc, t_map map, p_node parent);


/**
 * @brief Recherche la valeur minimale parmi toutes les feuilles de l'arbre.
 * @param t_node* node : Pointeur vers la racine de l'arbre ou sous-arbre.
 * @return int : Valeur minimale trouvée parmi les feuilles de l'arbre.
 */
int search_min(t_node* node);








#endif //UNTITLED1_TREE_H



