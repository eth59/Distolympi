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

