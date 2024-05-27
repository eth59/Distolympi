#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "structures.h"
#include "characters.h"
#include "collisionHandling.h"

void moveCharacter(Character *character, SDL_DisplayMode displayMode, float dx, float dy, char *collisionTable) {
    /*
    QUAND ON VEUT LA HAUTEUR DU PERSONNAGE IL FAUT FAIRE *1.4
    Pouquoi ?
    Parce que je ne sais pas.
    La hauteur est la bonne (sinon l'affichage du personnage serait déformé, disproportionné)
    Mais si on ne fait pas de multiplication, approximativement la moitié du perso sort de l'écran
    *1.4 c'est la valeur parfait pour arriver au bord de l'écran
    C et la SDL ou plutôt les trucs incompréhensibles 
    */

    // On initialise les variables qui vont vérifier si il y a collision
    int collisionX;
    int collisionXHeight;
    int collisionY;
    int collisionYWidth;
    // On check la direction du déplacement pour savoir quel coté de la hitbox on doit vérifier
    if (dx >= 0 && dy >= 0) {
        // on vérifie les collisions selon les axes x et y
        collisionX = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + character->hitBoxHeight + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + character->hitBoxHeight + dy);
    } else if (dx >= 0 && dy < 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + dy);
    } else if (dx < 0 && dy >= 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + character->hitBoxHeight + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + character->hitBoxHeight + dy);
    } else if (dx < 0 && dy < 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + dy);
    }

    // on ajuste la position du personnage en fonction
    if ((collisionX || collisionXHeight) && (!collisionY && !collisionYWidth)) {
        character->yHitBox += dy;
        character->y += dy;
    } else if ((!collisionX && !collisionXHeight) && (collisionY || collisionYWidth)) {
        character->xHitBox += dx;
        character->x += dx;
    } else if ((!collisionX && !collisionXHeight) && (!collisionY && !collisionYWidth)) {
        character->xHitBox += dx;
        character->yHitBox += dy;
        character->x += dx;
        character->y += dy;
    }
}

int get_direction_and_move(int key_up_pressed ,int key_down_pressed,int key_left_pressed,int key_right_pressed,Character *character, SDL_DisplayMode displayMode, char *collisionTable){
    int direction=0;
    // Déplacement du personnage
    // On multiplie par sqrt(2) en diagonale
    // pour éviter une impression de vitesse plus élevée
    if (key_left_pressed && key_up_pressed) {
        direction=5;
        moveCharacter(character, displayMode, -character->speed*0.7071f, -character->speed*0.7071f, collisionTable);
    } else if (key_left_pressed && key_down_pressed) {
        direction=3;
        moveCharacter(character, displayMode, -character->speed*0.7071f, character->speed*0.7071f, collisionTable);
    } else if (key_right_pressed && key_up_pressed) {
        direction=7;
        moveCharacter(character, displayMode, character->speed*0.7071f, -character->speed*0.7071f, collisionTable);
    } else if (key_right_pressed && key_down_pressed) {
        direction=9;
        moveCharacter(character, displayMode, character->speed*0.7071f, character->speed*0.7071f, collisionTable);
    } else if (key_left_pressed) {
        direction=4;
        moveCharacter(character, displayMode, -character->speed, 0, collisionTable);
    } else if (key_right_pressed) {
        direction=8;
        moveCharacter(character, displayMode, character->speed, 0, collisionTable);
    } else if (key_up_pressed) {
        direction=6;
        moveCharacter(character, displayMode, 0, -character->speed, collisionTable);
    } else if (key_down_pressed) {
        direction=2;
        moveCharacter(character, displayMode, 0, character->speed, collisionTable);
    }
    return direction;
}


SDL_Rect get_Rectdest_attacks(int destination, Character character) {
    SDL_Rect Rectdest;

    // Initialisation des valeurs par défaut
    Rectdest.x = 0;
    Rectdest.y = 0;
    Rectdest.w = 0;
    Rectdest.h = 0;

    // Calcul du Rectdest en fonction de la destination
    if (destination == 0) {
        Rectdest.x = (int)character.x - (int)character.width / 4;
        Rectdest.y = (int)character.y + (int)character.height;
        Rectdest.w = (int)character.width * 1.5;
        Rectdest.h = (int)character.height;
    }
    else if (destination == 1) {
        Rectdest.x = (int)character.x - (int)character.width;
        Rectdest.y = (int)character.y + (int)character.height;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height;
    }
    else if (destination == 2) {
        Rectdest.x = (int)character.x - (int)character.width;
        Rectdest.y = (int)character.y - (int)character.height / 4;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height * 1.5;
    }
    else if (destination == 3) {
        Rectdest.x = (int)character.x - (int)character.width;
        Rectdest.y = (int)character.y - (int)character.height;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height;
    }
    else if (destination == 4) {
        Rectdest.x = (int)character.x - (int)character.width / 4;
        Rectdest.y = (int)character.y - (int)character.height;
        Rectdest.w = (int)character.width * 1.5;
        Rectdest.h = (int)character.height;
    }
    else if (destination == 5) {
        Rectdest.x = (int)character.x + (int)character.width;
        Rectdest.y = (int)character.y - (int)character.height;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height;
    }
    else if (destination == 6) {
        Rectdest.x = (int)character.x + (int)character.width;
        Rectdest.y = (int)character.y - (int)character.height / 4;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height * 1.5;
    }
    else if (destination == 7) {
        Rectdest.x = (int)character.x + (int)character.width;
        Rectdest.y = (int)character.y + (int)character.height;
        Rectdest.w = (int)character.width;
        Rectdest.h = (int)character.height;
    }
    return Rectdest;
}


int get_melee_direction(int charX, int charY, int mouseX, int mouseY) {
    // Calcul des coordonnées relatives de la souris par rapport au personnage
    int relX = mouseX - charX;
    int relY = mouseY - charY;

    // Calcul de l'angle entre l'axe x positif et la ligne reliant le personnage et la souris
    double angle = atan2(relY, relX);
    
    // Si l'angle est négatif, le convertir en un angle positif entre 0 et 2*PI
    if (angle < 0) {
        angle += 2 * PI;
    }

    // Convertir l'angle en un index de direction (de 0 à 8)
    // On divise le cercle en 8 secteurs de 45 degrés chacun
    // Chaque secteur est associé à un entier de 0 à 7, donc on multiplie l'angle par 4/pi et on arrondit au plus proche
    int direction = (int)(angle * 4 / PI + 0.5);
    

    // Pour éviter que direction dépasse 8, on prend son modulo 8
    direction +=6;
    direction %= 8;

    return direction;
}

int get_CollisionInd(int key_up_pressed ,int key_down_pressed,int key_left_pressed,int key_right_pressed,Character *character,char* collisionTable){
    int dx;
    int dy;
    if (key_left_pressed && key_up_pressed) {
        dx =-character->speed*0.7071f;
        dy =-character->speed*0.7071f;
    } else if (key_left_pressed && key_down_pressed) {
        dx =-character->speed*0.7071f;
        dy = character->speed*0.7071f;
    } else if (key_right_pressed && key_up_pressed) {
        dx = character->speed*0.7071f;
        dy = -character->speed*0.7071f;
    } else if (key_right_pressed && key_down_pressed) {
        dx = character->speed*0.7071f;
        dy = character->speed*0.7071f;
    } else if (key_left_pressed) {
        dx =-character->speed;
        dy = 0;
    } else if (key_right_pressed) {
        dx =character->speed;
        dy = 0;
    } else if (key_up_pressed) {
        dx = 0;
        dy =  -character->speed;
    } else if (key_down_pressed) {
        dx = 0; 
        dy = character->speed;
    }
    int collisionX;
    int collisionXHeight;
    int collisionY;
    int collisionYWidth;
    // On check la direction du déplacement pour savoir quel coté de la hitbox on doit vérifier
    if (dx >= 0 && dy >= 0) {
        // on vérifie les collisions selon les axes x et y
        collisionX = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + character->hitBoxHeight + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + character->hitBoxHeight + dy);
    } else if (dx >= 0 && dy < 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + dy);
    } else if (dx < 0 && dy >= 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + character->hitBoxHeight + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + character->hitBoxHeight + dy);
    } else if (dx < 0 && dy < 0) {
        collisionX = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox);
        collisionXHeight = checkCollision(collisionTable, character->xHitBox + dx, character->yHitBox + character->hitBoxHeight);
        collisionY = checkCollision(collisionTable, character->xHitBox, character->yHitBox + dy);
        collisionYWidth = checkCollision(collisionTable, character->xHitBox + character->hitBoxWidth, character->yHitBox + dy);
    }
    return fmax(fmax(collisionX,collisionXHeight),fmax(collisionY,collisionYWidth));
}