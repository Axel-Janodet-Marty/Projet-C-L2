//
// Created by flasque on 19/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "loc.h"
#include "queue.h"
#include <time.h>

/* prototypes of local functions */
/* local functions are used only in this file, as helper functions */

/**
 * @brief :  function to get the position of the base station
 * @param map : the map
 * @return : the position of the base station
 */
t_position getBaseStationPosition(t_map);

/**
 * @brief : function to calculate costs of the map  from the base station
 * @param map : the map
 * @return none
 */
void calculateCosts(t_map);

/**
 * @brief : function to remove 'false' crevasses costs from the costs array
 * @param map : the map
 * @return none
 */
void removeFalseCrevasses(t_map);

/* definition of local functions */


t_map generateRandomMap(int xdim, int ydim) {
    t_map map;
    map.x_max = xdim;
    map.y_max = ydim;

    // Allocation de mémoire pour les sols
    map.soils = (t_soil **)malloc(ydim * sizeof(t_soil *));
    if (map.soils == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée pour map.soils\n");
        exit(1);
    }
    for (int i = 0; i < ydim; i++) {
        map.soils[i] = (t_soil *)malloc(xdim * sizeof(t_soil));
        if (map.soils[i] == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire échouée pour map.soils[%d]\n", i);
            exit(1);
        }
    }

    // Allocation de mémoire pour les coûts
    map.costs = (int **)malloc(ydim * sizeof(int *));
    if (map.costs == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée pour map.costs\n");
        exit(1);
    }
    for (int i = 0; i < ydim; i++) {
        map.costs[i] = (int *)malloc(xdim * sizeof(int));
        if (map.costs[i] == NULL) {
            fprintf(stderr, "Erreur : allocation mémoire échouée pour map.costs[%d]\n", i);
            exit(1);
        }
    }

    // Dimensions totales
    int total_cells = xdim * ydim;

    // Calculer les quantités de chaque type de sol
    int max_crevasses = total_cells * 0.05; // Max 5% de crevasses
    int crevasse_count = (total_cells > max_crevasses) ? max_crevasses : total_cells / 10; // Max 5 ou 10% des cases
    int plain_count = total_cells / 2; // 50% de plaines
    int erg_reg_count = (total_cells - plain_count - crevasse_count) / 2; // Le reste pour ergs et regs

    // Création d'une liste aléatoire de sols
    t_soil *shuffled_soils = (t_soil *)malloc(total_cells * sizeof(t_soil));
    if (shuffled_soils == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée pour shuffled_soils\n");
        exit(1);
    }

    // Ajouter les types de sols en respectant les quantités
    for (int i = 0; i < crevasse_count; i++) shuffled_soils[i] = CREVASSE;
    for (int i = crevasse_count; i < crevasse_count + plain_count; i++) shuffled_soils[i] = PLAIN;
    for (int i = crevasse_count + plain_count; i < crevasse_count + plain_count + erg_reg_count; i++) shuffled_soils[i] = ERG;
    for (int i = crevasse_count + plain_count + erg_reg_count; i < total_cells; i++) shuffled_soils[i] = REG;

    // Mélanger les sols
    srand(time(NULL));
    for (int i = total_cells - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        t_soil temp = shuffled_soils[i];
        shuffled_soils[i] = shuffled_soils[j];
        shuffled_soils[j] = temp;
    }

    // Remplir la carte avec les sols mélangés
    for (int i = 0; i < ydim; i++) {
        for (int j = 0; j < xdim; j++) {
            map.soils[i][j] = shuffled_soils[i * xdim + j];
            map.costs[i][j] = COST_UNDEF; // Coût par défaut
        }
    }

    free(shuffled_soils);

    // Placement de la base station
    int base_x = rand() % xdim;
    int base_y = rand() % ydim;
    map.soils[base_y][base_x] = BASE_STATION;
    map.costs[base_y][base_x] = 0;

    // Calcul des coûts et suppression des faux crevasses
    calculateCosts(map);
    removeFalseCrevasses(map);

    return map;
}



t_position getBaseStationPosition(t_map map)
{
    t_position pos;
    int i = 0;
    int found = 0;
    while (i < map.y_max && !found)
    {
        int j = 0;
        while (j < map.x_max && !found)
        {
            if (map.soils[i][j] == BASE_STATION)
            {
                pos.x = j;
                pos.y = i;
                found = 1;
            }
            j++;
        }
        i++;
    }
    // if the base station is not found, we exit the program
    if (!found)
    {
        fprintf(stderr, "Error: base station not found in the map\n");
        exit(1);
    }
    return pos;
}

void removeFalseCrevasses(t_map map)
{
    int over = 0;

    while (!over)
    {
        int min_cost = COST_UNDEF;
        int imin = -1, jmin = -1;

        // Trouver la position avec le coût minimum valide (> 10000) et pas une crevasse
        for (int i = 0; i < map.y_max; i++)
        {
            for (int j = 0; j < map.x_max; j++)
            {
                if (map.soils[i][j] != CREVASSE && map.costs[i][j] > 10000 && map.costs[i][j] < min_cost)
                {
                    min_cost = map.costs[i][j];
                    imin = i;
                    jmin = j;
                }
            }
        }

        // Si aucune position valide n'a été trouvée, la boucle peut s'arrêter
        if (imin == -1 || jmin == -1)
        {
            over = 1;
            continue;
        }

        // Calculer les coûts des voisins de cette position
        t_position pos = {jmin, imin};
        t_position lp = LEFT(pos), rp = RIGHT(pos), up = UP(pos), dp = DOWN(pos);
        int min_neighbour = COST_UNDEF;

        if (isValidLocalisation(lp, map.x_max, map.y_max))
            min_neighbour = (map.costs[lp.y][lp.x] < min_neighbour) ? map.costs[lp.y][lp.x] : min_neighbour;

        if (isValidLocalisation(rp, map.x_max, map.y_max))
            min_neighbour = (map.costs[rp.y][rp.x] < min_neighbour) ? map.costs[rp.y][rp.x] : min_neighbour;

        if (isValidLocalisation(up, map.x_max, map.y_max))
            min_neighbour = (map.costs[up.y][up.x] < min_neighbour) ? map.costs[up.y][up.x] : min_neighbour;

        if (isValidLocalisation(dp, map.x_max, map.y_max))
            min_neighbour = (map.costs[dp.y][dp.x] < min_neighbour) ? map.costs[dp.y][dp.x] : min_neighbour;

        // Mettre à jour le coût de la case actuelle
        int self_cost = _soil_cost[map.soils[imin][jmin]];
        int new_cost = min_neighbour + self_cost;

        // Vérification pour éviter les cycles
        if (new_cost < map.costs[imin][jmin])
        {
            map.costs[imin][jmin] = new_cost;
        }
        else
        {
            over = 1; // Arrêt si aucune mise à jour n'est possible
        }
    }
}

void calculateCosts(t_map map)
{
    // Obtenir la position de la station de base
    t_position baseStation = getBaseStationPosition(map);

    // Créer une file pour gérer les positions à visiter
    t_queue queue = createQueue(map.x_max * map.y_max);

    // Ajouter la station de base à la file
    enqueue(&queue, baseStation);

    // Tant que la file n'est pas vide
    while (queue.first != queue.last)
    {
        // Extraire la position courante
        t_position pos = dequeue(&queue);

        // Récupérer le coût propre à cette case
        int self_cost = _soil_cost[map.soils[pos.y][pos.x]];

        // Définir les voisins de la position actuelle
        t_position lp = LEFT(pos), rp = RIGHT(pos), up = UP(pos), dp = DOWN(pos);
        t_position neighbors[4] = {lp, rp, up, dp};

        // Parcourir les voisins
        for (int k = 0; k < 4; k++)
        {
            t_position neighbor = neighbors[k];

            // Vérifier si le voisin est dans les limites
            if (isValidLocalisation(neighbor, map.x_max, map.y_max))
            {
                int current_cost = map.costs[neighbor.y][neighbor.x];
                int new_cost = map.costs[pos.y][pos.x] + _soil_cost[map.soils[neighbor.y][neighbor.x]];

                // Mettre à jour le coût si nécessaire
                if (current_cost == COST_UNDEF || new_cost < current_cost)
                {
                    map.costs[neighbor.y][neighbor.x] = new_cost;
                    enqueue(&queue, neighbor);
                }
            }
        }
    }
}
/* definition of exported functions */

t_map createMapFromFile(char *filename)
{
    /* rules for the file :
     * - the first line contains the number of lines : y dimension (int)
     * - the second line contains the number of columns : x dimension (int)
     * - the next lines contain the map values (int) separated by spaces : one line = one row
     * - the values are the following : 0 = BASE_STATION, 1 = PLAIN, 2 = ERG, 3 = REG, 4 = CREVASSE
     */

    t_map map;
    int xdim, ydim;     // dimensions of the map
    char buffer[100];   // buffer for reading the file line by line

    FILE *file = fopen(filename,"rt");
    if (file == NULL)
    {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(1);
    }
    fscanf(file, "%d", &ydim);
    fscanf(file, "%d", &xdim);
    map.x_max = xdim;
    map.y_max = ydim;
    map.soils = (t_soil **)malloc(ydim * sizeof(t_soil *));
    for (int i = 0; i < ydim; i++)
    {
        map.soils[i] = (t_soil *)malloc(xdim * sizeof(t_soil));
    }
    map.costs = (int **)malloc(ydim * sizeof(int *));
    for (int i = 0; i < ydim; i++)
    {
        map.costs[i] = (int *)malloc(xdim * sizeof(int));
    }
    for (int i = 0; i < ydim; i++)
    {

        // parse the line to get the values : 0 = BASE_STATION, 1 = PLAIN, 2 = ERG, 3 = REG, 4 = CREVASSE
        // values are separated by spaces, so we use sscanf with %d to get the values
        for (int j = 0; j < xdim; j++)
        {
            int value;
            fscanf(file, "%d", &value);
            map.soils[i][j] = value;
            // cost is 0 for BASE_STATION, 65535 for other soils
            map.costs[i][j] = (value == BASE_STATION) ? 0 : COST_UNDEF;
        }

    }
    fclose(file);
    calculateCosts(map);
    removeFalseCrevasses(map);
    return map;
}

t_map createTrainingMap()
{
    return createMapFromFile("..\\maps\\training.map");
}

void displayMap(t_map map)
{
    /** the rules for display are :
     * display all soils with 3x3 characters
     * characters are : B for base station, '-' for plain, '~' for erg, '^' for reg, ' ' for crevasse
     */
    for (int i = 0; i < map.y_max; i++)
    {
        for (int rep = 0; rep < 3; rep++)
        {
            for (int j = 0; j < map.x_max; j++)
            {
                char c[4];
                switch (map.soils[i][j])
                {
                    case BASE_STATION:
                        if (rep==1)
                        {
                            strcpy(c, " B ");
                        }
                        else
                        {
                            strcpy(c, "   ");
                        }
                        break;
                    case PLAIN:
                        strcpy(c, "---");
                        break;
                    case ERG:
                        strcpy(c, "~~~");
                        break;
                    case REG:
                        strcpy(c, "^^^");
                        break;
                    case CREVASSE:
                        sprintf(c, "###");
                        break;
                    default:
                        strcpy(c, "???");
                        break;
                }
                printf("%s", c);
            }
            printf("\n");
        }

    }
    return;
}

const char* getSoilAsString(t_soil soil) {
    switch (soil) {
        case BASE_STATION:
            return "Base Station";
        case PLAIN:
            return "Plain";
        case ERG:
            return "Erg";
        case REG:
            return "Reg";
        case CREVASSE:
            return "Crevasse";
        default:
            return "Unknown Soil";
    }
}
