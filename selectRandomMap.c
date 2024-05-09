#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int randomMapIndex() {
    //initialisation de la seed pour selection aléatoire
    srand(time(NULL));

    //génération d'un nombre aléatoire entre 0 et 1
    int randomNumber = rand() % 2;

    randomNumber += 1;

    return randomNumber;
}

char *getMapFromIndex(int index) {
    //initialise la chaine qui contient l'extension
    char ext[] = ".png";
    //intialise la variable qui contient l'index en chaine de caractère
    char charIndex[2]; 
    //transforme l'index en chaine de caractères
    sprintf(charIndex, "%d", index);

    // Allouer de la mémoire pour stocker la chaîne résultante
    
    char *map = malloc(18 * sizeof(char)); // "map" + chiffre + ".png" + terminateur nul

    //ajoute le "map" a la chaine de caractère
    if (index < 10) {
        strcpy(map, "assets/map0");
    } else {
        strcpy(map, "assets/map");
    }
    //concatène toutes les chaines
    strcat(map, charIndex);
    strcat(map, ext);

    return map;
}

char *getColliderTable(int index) {
    
    //initialise la chaine qui contient l'extension
    char ext[] = ".txt";
    //intialise la variable qui contient l'index en chaine de caractère
    char charIndex[2]; 
    //transforme l'index en chaine de caractères 
    sprintf(charIndex, "%d", index);

    // Allouer de la mémoire pour stocker la chaîne résultante
    char *colliderFile = malloc(18 * sizeof(char)); // "map" + chiffre + ".txt" + terminateur nul

    //ajoute le "map" a la chaine de caractère
    if (index < 10) {
        strcpy(colliderFile, "assets/map0");
    } else {
        strcpy(colliderFile, "assets/map");
    }
    //concatène toutes les chaines
    strcat(colliderFile, charIndex);
    strcat(colliderFile, ext);

    return colliderFile;    
}

char *getMapHoles(int index) {
    //initialise la chaine qui contient l'extension
    char ext[] = ".txt";
    //intialise la variable qui contient l'index en chaine de caractère
    char charIndex[2]; 
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

    // Comptons le nombre de nombres dans la chaîne de caractères
    *taille = 0;
    for (int i = 0; i < tailleFichier; i++) {
        if (contenu[i] >= '0' && contenu[i] <= '9') {
            (*taille)++;
            while (contenu[i] >= '0' && contenu[i] <= '9') {
                i++;
            }
        }
    }

    // Allocation de mémoire pour le tableau d'entiers
    int *tableau = (int *)malloc(*taille * sizeof(int));
    if (tableau == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(1);
    }

    // Lecture des nombres depuis la chaîne de caractères et stockage dans le tableau
    int index = 0;
    char *nombre = strtok(contenu, " ,[]");
    while (nombre != NULL) {
        tableau[index++] = atoi(nombre);
        nombre = strtok(NULL, " ,[]");
    }

    // Libération de la mémoire allouée pour la chaîne de caractères
    free(contenu);

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
