#include "tree.h"

p_node Init_Node(int nb_sons, int depth, t_move move, t_localisation loc, t_map map, p_node node){
    p_node new_node;
    new_node = (t_node *)malloc(sizeof(t_node));
    new_node->value = map.costs[loc.pos.y][loc.pos.x];
    new_node->depth = depth;
    new_node->move = move;
    new_node->parent = node;
    new_node->nbSons = nb_sons;
    new_node->sons = (t_node **)malloc(nb_sons*sizeof(t_node *));
    for (int i = 0; i < nb_sons; i++)
    {
        new_node->sons[i] = NULL;
    }
    return new_node;
}

p_node BuildTree(int NB_Moves, int depth, t_move move, t_move* tab_moves, t_localisation loc, t_map map, p_node parent){
    
    if (depth > NB_choices) {
        return NULL;
    }
    else if (depth == NB_choices || map.costs[loc.pos.y][loc.pos.x] >= 10000 || map.costs[loc.pos.y][loc.pos.x] == 0) {
        NB_Moves = 0;
    }

    p_node node = Init_Node(NB_Moves, depth, move, loc, map, parent);

    for (int i = 0; i < NB_Moves; i++) {

        t_localisation new_loc = loc;
        updateLocalisation(&new_loc, tab_moves[i]);

        if (isValidLocalisation(new_loc.pos, map.x_max, map.y_max)) {

            t_move* new_tab_moves = update_movesList(tab_moves, NB_Moves, i);
            node->sons[i] = BuildTree(NB_Moves - 1, depth+1, tab_moves[i], new_tab_moves, new_loc, map, node); 
            free(new_tab_moves);
        }
        else{                                                                                                        
            node->sons[i] = NULL;
        }
    }
    return node;
}



int search_min(p_node node){  
    int min = node->value;

    if (node->nbSons!=0){
        for (int i = 0; i < node->nbSons; i++) {
            if (node->sons[i]!=NULL){
                int min_son = search_min(node->sons[i]);
                if (min_son < min){
                    min = min_son;
                }
            }
        }
    }
    return min;
}


void explore(t_node* node, t_node** best_min, int* min_value, int* min_depth) { // Fonction complémentaire pour cherchee le best min node
    if (node == NULL) {
        return;
    }

    // Si on trouve une nouvelle valeur minimale ou un nœud avec une profondeur plus faible pour la même valeur minimale
    if (node->value < *min_value || (node->value == *min_value && node->depth < *min_depth)) {
        *min_value = node->value;
        *min_depth = node->depth;
        *best_min = node;
    }

    // Parcourir les fils du nœud actuel
    for (int i = 0; i < node->nbSons; i++) {
        explore(node->sons[i], best_min, min_value, min_depth);
    }
}

// Fonction principale pour trouver le meilleur minimum
t_node* find_best_min(t_node* root) {
    if (root == NULL) {
        return NULL; // Retourne NULL si l'arbre est vide
    }

    t_node* best_min = NULL; // Pointeur pour stocker le meilleur minimum trouvé
    int min_value = root->value; // Initialiser avec la valeur du nœud racine
    int min_depth = root->depth; // Initialiser avec la profondeur du nœud racine

    // Appeler la fonction d'exploration
    explore(root, &best_min, &min_value, &min_depth);

    return best_min;
}

t_move* get_moves_from_start(t_node* node, int* move_count) {
    if (node == NULL) {
        *move_count = 0;
        return NULL; // Si le nœud est NULL, retourne un tableau vide
    }

    // Compter le nombre de mouvements en remontant jusqu'à la racine
    int count = 0;
    t_node* current = node;
    while (current != NULL) {
        count++;
        current = current->parent;
    }

    // Allouer un tableau pour stocker les mouvements
    t_move* moves = (t_move*)malloc(count * sizeof(t_move));
    if (moves == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Remplir le tableau en remontant de la feuille à la racine
    current = node;
    for (int i = count - 1; i >= 0; i--) {
        moves[i] = current->move;
        current = current->parent;
    }

    *move_count = count; // Retourner le nombre de mouvements
    return moves;
}

void print_moves(t_move* moves, int move_count) {
    if (moves == NULL || move_count == 0) {
        printf("Aucun mouvement effectué.\n");
        return;
    }
    printf("Start");

    for (int i = 0; i < move_count; i++) {
        printf("%s", getMoveAsString(moves[i])); // Utilise la fonction fournie
        if (i != move_count - 1) {
            printf(" -> "); // Ajoute une flèche entre les mouvements
        }
    }
    printf(" (End)");
    printf("\n");
}



void printTree(t_node *node, int *architecture, int depth) {
    if (!node) return; // Vérifie si le nœud est NULL

    // Affiche les espaces pour indenter correctement l'arbre
    for (int i = 0; i < depth; i++) {
        if (architecture[i]) {
            printf("|   ");
        } else {
            printf("    ");
        }
    }

    // Détermine si c'est le dernier fils
    int sonsPass = 0;
    for (int i = 0; i < node->depth; i++) {
        if (node->sons[i] != NULL) {
            sonsPass++;
        }
    }

    // Affichage de la structure de l'arbre
    if (sonsPass + 1 == node->nbSons) {
        printf("`-- %d\n", node->value);
        architecture[depth] = 0;
    } else {
        printf("|-- %d\n", node->value);
        architecture[depth] = 1;
    }

    // Récursion pour afficher les fils
    for (int i = 0; i < node->nbSons; i++) {
        if (node->sons[i] != NULL) {
            printTree(node->sons[i], architecture, depth + 1);
        }
    }
}
