#include <stdio.h>
#include <stdlib.h>

char *openCollisionFile(char *fileName) {
    
    FILE *file;
    // Ouvrir le fichier 
    file = fopen(fileName, "r");

    // On determine la taille du fichier
    int length;
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // chaine de caractère qui contiendra toutes les infos sur les collisions possibles
    char *collisionTable = (char *)malloc((length + 1) * sizeof(char));

    // on ajoute tous les caractères du fichier a la chaine
    int i = 0;
    int character;
    while ((character = fgetc(file)) != EOF) { 
        collisionTable[i] = (char)character;
        i ++; 
    }
    collisionTable[i] = '\0'; 

    fclose(file);

    return collisionTable;
}

int main() {
    char *fileName = "assets/map01.txt";
    char *collisionTable;
    collisionTable = openCollisionFile(fileName);
    return 0;

}