#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "randomSpawn.h"

char *getMapHoles(int index) {
    //initialise la chaine qui contient l'extension
    char ext[] = ".txt";
    //intialise la variable qui contient l'index en chaine de caractère
    int temp;
    if (index >= 10) {
        temp = 3;
    } else {
        temp = 2;
    }
    char charIndex[temp]; 
    //transforme l'index en chaine de caractères 
    sprintf(charIndex, "%d", index);

    // Allouer de la mémoire pour stocker la chaîne résultante
    char *mapHoles = malloc(23 * sizeof(char)); // "map" + chiffre + ".txt" + terminateur nul
    if (index < 10) {
        strcpy(mapHoles, "assets/mapholes0");
    } else {
        strcpy(mapHoles, "assets/mapholes");
    }
    //concatène toutes les chaines
    strcat(mapHoles, charIndex);
    strcat(mapHoles, ext);

    return mapHoles;  
}

int *convertirFichierEnTableau(const char *nomFichier, int *taille) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", nomFichier);
        exit(1);
    }

    // Obtention de la taille du fichier
    fseek(fichier, 0, SEEK_END);
    long tailleFichier = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    // Allocation de mémoire pour stocker le contenu du fichier
    char *contenu = (char *)malloc(tailleFichier * sizeof(char));
    if (contenu == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }

    // Lecture du contenu du fichier dans une chaîne de caractères
    fread(contenu, sizeof(char), tailleFichier, fichier);
    fclose(fichier);

    // Création d'une copie de la chaîne de caractères lue à partir du fichier
    char *copie = (char *)malloc((tailleFichier + 1) * sizeof(char));
    if (copie == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }
    strncpy(copie, contenu, tailleFichier);
    copie[tailleFichier] = '\0';

    // Libération de la mémoire allouée pour la variable contenu
    free(contenu);

    // Comptons le nombre de nombres dans la chaîne de caractères
    *taille = 0;
    for (int i = 0; i < tailleFichier; i++) {
        if (copie[i] >= '0' && copie[i] <= '9') {
            (*taille)++;
            while (copie[i] >= '0' && copie[i] <= '9') {
                i++;
            }
        }
    }
    // Allocation de mémoire pour le tableau d'entiers
    int *tableau = (int *)malloc(*taille *sizeof(int));
    if (tableau == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }

    // Lecture des nombres depuis la chaîne de caractères et stockage dans le tableau
    int index = 0;
    char *nombre = strtok(copie, " ,[]");
    while (nombre != NULL) {
        tableau[index++] = atoi(nombre);
        nombre = strtok(NULL, " ,[]");
    }

    // Libération de la mémoire allouée pour la copie de la chaîne de caractères
    free(copie);

    return tableau;
}


void getCoordFromTiles(int tileNumber, int *xTile, int *yTile) {
    *xTile = tileNumber%16;
    *yTile = tileNumber/16;
}


int randomInt(int max) {
    srand(time(NULL));
    return rand()%max;
}
