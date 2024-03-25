#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    if ((collisionX || collisionXHeight) && (!collisionY && collisionYWidth)) {
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


SDL_Rect get_Rectdest_attacks(int destination,Character character){
    destination=destination-2;
    SDL_Rect Rectdest;
    if(destination==0){SDL_Rect Rectdest = {(int)character.x-(int)character.width/4, (int)character.y+(int)character.height, (int)character.width*1.5, (int)character.height};}
    else if(destination==1){SDL_Rect Rectdest = {(int)character.x-(int)character.width, (int)character.y+(int)character.height, (int)character.width, (int)character.height};}
    else if(destination==2){SDL_Rect Rectdest = {(int)character.x-(int)character.width, (int)character.y-(int)character.height/4, (int)character.width, (int)character.height*1.5};}
    else if(destination==3){SDL_Rect Rectdest = {(int)character.x-(int)character.width, (int)character.y-(int)character.height, (int)character.width, (int)character.height};}
    else if(destination==4){SDL_Rect Rectdest = {(int)character.x-(int)character.width/4, (int)character.y-(int)character.height, (int)character.width*1.5, (int)character.height};}
    else if(destination==5){SDL_Rect Rectdest = {(int)character.x+(int)character.width, (int)character.y-(int)character.height, (int)character.width, (int)character.height};}
    else if(destination==6){SDL_Rect Rectdest = {(int)character.x+(int)character.width, (int)character.y-(int)character.height/4, (int)character.width, (int)character.height*1.5};}
    else if(destination==7){SDL_Rect Rectdest = {(int)character.x+(int)character.width, (int)character.y+(int)character.height, (int)character.width, (int)character.height};}
    return Rectdest;
}