#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structures.h"
#include "characters.h"



void moveCharacter(Character *character, Wall wall, SDL_DisplayMode displayMode, float dx, float dy) {

    // On commence par calculer la valeur des bordures
    int border_left = wall.left; // 0 sans mur
    int border_up = wall. up; // 0 sans mur
    int border_right = displayMode.w - character->width - wall.right; // on soustrait la largeur du perso pour pas qu'il sorte de l'écran
    int border_down = displayMode.h - 1.4 * character->height - wall.down; // on soustrait la hauteur du perso pour pas qu'il sorte de l'écran (avec le fameux 1.4)

    // On teste les déplacements pour pas que ça sorte des bordures qu'on vient de calculer
    if (character->x + dx < border_left && character->y + dy < border_up)
    {
        // Coin en haut à gauche
        character->x = border_left;
        character->y = border_up;
    } else if (character->x + dx > border_right && character->y + dy < border_up)
    {
        // Coin en haut à droite
        character->x = border_right;
        character->y = border_up;
    } else if (character->x + dx > border_right && character->y + dy > border_down)
    {
        // Coin en bas à droit
        character->x = border_right;
        character->y = border_down;
    } else if (character->x + dx < border_left && character->y + dy > border_down)
    {
        // Coin en bas à gauche
        character->x = border_left;
        character->y = border_down;
    } else if (character->x + dx < border_left)
    {
        // Mur gauche
        character->x = border_left;
        character->y += dy;
    } else if (character->y + dy < border_up)
    {
        // Mur haut
        character->x += dx;
        character->y = border_up;
    } else if (character->x + dx > border_right)
    {
        // Mur droite
        character->x = border_right;
        character->y += dy;
    } else if (character->y + dy > border_down)
    {
        // Mur bas
        character->x += dx;
        character->y = border_down;
    } else 
    {
        // Aucun mur sur le chemin
        character->x += dx;
        character->y += dy;
    }
}

int get_direction_and_move(int key_up_pressed ,int key_down_pressed,int key_left_pressed,int key_right_pressed,Character *character,Wall wall, SDL_DisplayMode displayMode){
    int direction=0;
    // Déplacement du personnage
    // On multiplie par sqrt(2) en diagonale
    // pour éviter une impression de vitesse plus élévée
    if (key_left_pressed && key_up_pressed) {
        direction=5;
        moveCharacter(character, wall, displayMode, -character->speed*0.7071f, -character->speed*0.7071f);
    } else if (key_left_pressed && key_down_pressed) {
        direction=3;
        moveCharacter(character, wall, displayMode, -character->speed*0.7071f, character->speed*0.7071f);
    } else if (key_right_pressed && key_up_pressed) {
        direction=7;
        moveCharacter(character, wall, displayMode, character->speed*0.7071f, -character->speed*0.7071f);
    } else if (key_right_pressed && key_down_pressed) {
        direction=9;
        moveCharacter(character, wall, displayMode, character->speed*0.7071f, character->speed*0.7071f);
    } else if (key_left_pressed) {
        direction=4;
        moveCharacter(character, wall, displayMode, -character->speed, 0);
    } else if (key_right_pressed) {
        direction=8;
        moveCharacter(character, wall, displayMode, character->speed, 0);
    } else if (key_up_pressed) {
        direction=6;
        moveCharacter(character, wall, displayMode, 0, -character->speed);
    } else if (key_down_pressed) {
        direction=2;
        moveCharacter(character, wall, displayMode, 0, character->speed);
    }
    return direction;
}