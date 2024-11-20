# Projet MARC - Navigation du Rover      *{SKANDER HADMOUNI, AXEL JANODET--MARTY, SAMI BENABDALLAH}*
Objectif du projet Rover Marc :
Permettre à Marc de retourner à sa base malgrès des restrictions sur les mouvements.
# ***Fonctionnalités:***
**Création d'une carte** : Lecture d'une carte contenant des informations sur les sols et leurs coûts associés.
**Génération de mouvements** : Création de mouvements aléatoires en fonction de probabilités définies.
**Construction d'un arbre** : Représentation des déplacements possibles sous forme d'arbre.
**Recherche de minimum** : Identification de la feuille ayant la valeur minimale parmi toutes les feuilles de l'arbre.
**Calcul du chemin optimal** : Récupération des mouvements depuis la racine jusqu'à la feuille de coût minimal.
**Simulation de guidage** : Affichage des mouvements pour guider le rover jusqu'à la station de base. 

.
├── main.c          # Point d'entrée du programme
├── tree.c          # Implémentation des structures et algorithmes pour l'arbre
├── tree.h          # Déclarations des fonctions et structures pour l'arbre
├── map.c           # Gestion des cartes
├── map.h           # Déclarations des fonctions pour les cartes
├── moves.c         # Génération et gestion des mouvements
├── moves.h         # Déclarations des fonctions pour les mouvements
├── stack.c         # Implémentation des piles pour des parcours de structures
├── stack.h         # Déclarations des fonctions pour les piles
├── README.md       # Documentation du projet
