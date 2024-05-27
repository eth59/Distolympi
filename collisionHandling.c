#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define TILE_NUMBER_X 16
#define TILE_NUMBER_Y 9

char *openCollisionFile(char *fileName) {
    
    FILE *file;
    // Ouvrir le fichier 
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("erreur");
        return NULL;
    }
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

int checkCollision(char *collisionTable, float x, float y) {
    
    // tileX et tileY vont contenir les coordonnées de la tile sur laquelle on se trouve
    float tileX;
    float tileY;
    
    // on calcule la largeur et la hauteur des tiles
    float tileWidth = SCREEN_WIDTH / TILE_NUMBER_X;
    float tileHeigth = SCREEN_HEIGHT / TILE_NUMBER_Y;

    tileX = x/tileWidth;
    tileY = y/tileHeigth;

    int tileXIndex = floor(tileX);
    
    int tileYIndex = floor(tileY);

    // on calcule l'indice de la tile correspondante aux coordonnées
    int tileIndex = tileXIndex + 16*tileYIndex;

    // on vérifie si il y a collision
    if (collisionTable[tileIndex] == '0') {
        return 0;
    }
    if (collisionTable[tileIndex] == '2') {
        return 2;
    } 
    else {
        return 1;
    }    
    return 0;
}
