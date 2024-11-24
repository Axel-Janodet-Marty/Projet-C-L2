//
// Created by flasque on 19/10/2024.
//

#include "loc.h"
#include "map.h"
#include <ctype.h>
#include <string.h>

t_localisation loc_init(int x, int y, t_orientation ori)
{
    t_localisation loc;
    loc.pos.x = x;
    loc.pos.y = y;
    loc.ori = ori;
    return loc;
}

int isValidLocalisation(t_position loc, int x_max, int y_max)
{
    return (loc.x >= 0 && loc.x < x_max && loc.y >= 0 && loc.y < y_max);
}

t_position LEFT(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x - 1;
    new_pos.y = pos.y;
    return new_pos;
}

t_position RIGHT(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x + 1;
    new_pos.y = pos.y;
    return new_pos;
}

t_position UP(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x;
    new_pos.y = pos.y - 1;
    return new_pos;
}

t_position DOWN(t_position pos)
{
    t_position new_pos;
    new_pos.x = pos.x;
    new_pos.y = pos.y + 1;
    return new_pos;
}

void checkWin(int min){
    if (min==0){
        printf("Marc est rentré à la base, Terminé...\n");
    }
    else{
        printf("Contact perdu avec Marc, Terminé...\n");
    }
}

t_localisation inputLocalisation(t_map map) {
    t_localisation loc;
    int x, y;
    int orientation;
    char input[100];
    char orientationInput[10];

    // Vérification de la coordonnée x
    while (1) {
        printf("Entrez la position initiale x : ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Erreur de lecture. Réessayez.\n");
            continue;
        }
        if (sscanf(input, "%d", &x) != 1) {
            printf("Entrée invalide pour la coordonnée x. Réessayez.\n");
            continue;
        }
        if (x < 0 || x >= map.x_max) {
            printf("Coordonnée x invalide. Doit être entre 0 et %d.\n", map.x_max - 1);
        } else {
            break; // x valide
        }
    }

    // Vérification de la coordonnée y
    while (1) {
        printf("Entrez la position initiale y : ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Erreur de lecture. Réessayez.\n");
            continue;
        }
        if (sscanf(input, "%d", &y) != 1) {
            printf("Entrée invalide pour la coordonnée y. Réessayez.\n");
            continue;
        }
        if (y < 0 || y >= map.y_max) {
            printf("Coordonnée y invalide. Doit être entre 0 et %d.\n", map.y_max - 1);
        } else {
            break; // y valide
        }
    }

    // Vérification de l'orientation
    while (1) {
        printf("Entrez l'orientation initiale (NORTH, EAST, SOUTH, WEST) : ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Erreur de lecture. Réessayez.\n");
            continue;
        }
        if (sscanf(input, "%9s", orientationInput) != 1) {
            printf("Entrée invalide pour l'orientation. Réessayez.\n");
            continue;
        }

        // Convertir en majuscules pour simplifier la comparaison
        for (int i = 0; orientationInput[i]; i++) {
            orientationInput[i] = toupper(orientationInput[i]);
        }

        if (strcmp(orientationInput, "NORTH") == 0) {
            orientation = NORTH;
            break;
        } else if (strcmp(orientationInput, "EAST") == 0) {
            orientation = EAST;
            break;
        } else if (strcmp(orientationInput, "SOUTH") == 0) {
            orientation = SOUTH;
            break;
        } else if (strcmp(orientationInput, "WEST") == 0) {
            orientation = WEST;
            break;
        } else {
            printf("Orientation invalide. Choisissez parmi NORTH, EAST, SOUTH, WEST.\n");
        }
    }

    // Initialiser la localisation avec les entrées validées
    loc = loc_init(x, y, orientation);

    printf("Position initialisée : (%d:%d), Orientation : %s\n",
           loc.pos.x, loc.pos.y,
           (orientation == NORTH ? "NORTH" :
            orientation == EAST ? "EAST" :
            orientation == SOUTH ? "SOUTH" :
            "WEST"));
    return loc;
}
