#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "animations.h"
#include "structures.h"
#include "enemies.h"

void init_zombie(Enemy **zombie, SDL_Renderer *renderer, SDL_Texture **zombieTexture, SDL_Rect **zombieRectSrc)
{
    // Chargement de la texture
    *zombieTexture = get_texture("assets/zombie.png", renderer);

    // Création de l'objet
    *zombie = malloc(sizeof(Enemy));
    (*zombie)->x = 100;
    (*zombie)->y = 100;
    (*zombie)->width = 130;
    (*zombie)->height = 130;
    (*zombie)->speed = 1.5;
    (*zombie)->direction = 0;
    (*zombie)->animation_frame = 0;
    (*zombie)->delay_frame = 0;
    (*zombie)->max_column_frame = 8;
    (*zombie)->max_line_frame = 10;
    (*zombie)->attack_damage = 10;
    (*zombie)->health = 20;
    
    // On divise le tileset du zombie
    *zombieRectSrc = get_frames(32, 32, (*zombie)->max_line_frame, (*zombie)->max_column_frame);
}

void render_zombie(Enemy *zombie, SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRectSrc)
{
    SDL_Rect zombieRectDest = {(int)zombie->x, (int)zombie->y, zombie->width, zombie->height};
    int sprite_nb = zombie->direction * zombie->max_column_frame + zombie->animation_frame;
    SDL_RenderCopy(renderer, zombieTexture, &zombieRectSrc[sprite_nb], &zombieRectDest);
}

void move_zombie(Enemy **zombie, Character *character)
{
    float dx = character->x - (*zombie)->x;
    float dy = character->y - (*zombie)->y;

    // Calcul de l'angle entre le zombie et le joueur
    // J'ai une erreur sur vscode perso pour M_PI mais il est bien défini dans math.h pas d'inquiétude
    float angle = atan2(dy, dx) * 180 / M_PI; 

    // Convertion de l'angle en une direction + déplacement du zombie
    // On multiplie par sqrt(2) en diagonale
    // pour éviter une impression de vitesse plus élevée
    if (angle >= -22.5 && angle < 22.5) {
        // Droite
        (*zombie)->direction = 8;
        (*zombie)->x += (*zombie)->speed;
    } else if (angle >= 22.5 && angle < 67.5) {
        // Bas droite
        (*zombie)->direction = 9;
        (*zombie)->x += (*zombie)->speed*0.7071f;
        (*zombie)->y += (*zombie)->speed*0.7071f;
    } else if (angle >= 67.5 && angle < 112.5) {
        // Bas
        (*zombie)->direction = 2;
        (*zombie)->y += (*zombie)->speed;
    } else if (angle >= 112.5 && angle < 157.5) {
        // Bas gauche
        (*zombie)->direction = 3;
        (*zombie)->x -= (*zombie)->speed*0.7071f;
        (*zombie)->y += (*zombie)->speed*0.7071f;
    } else if (angle >= 157.5 || angle < -157.5) {
        // Gauche
        (*zombie)->direction = 4;
        (*zombie)->x -= (*zombie)->speed;
    } else if (angle >= -157.5 && angle < -112.5) {
        // Haut gauche
        (*zombie)->direction = 5;
        (*zombie)->x -= (*zombie)->speed*0.7071f;
        (*zombie)->y -= (*zombie)->speed*0.7071f;
    } else if (angle >= -112.5 && angle < -67.5) {
        // Haut
        (*zombie)->direction = 6;
        (*zombie)->y -= (*zombie)->speed;
    } else {
        // Haut droite
        (*zombie)->direction = 7;
        (*zombie)->x += (*zombie)->speed*0.7071f;
        (*zombie)->y -= (*zombie)->speed*0.7071f;
    }

    // On met à jour les paramètres de l'animation
    if ((*zombie)->delay_frame > 20 / (*zombie)->speed)
    {
        (*zombie)->animation_frame = ((*zombie)->animation_frame + 1) % (*zombie)->max_column_frame;
        (*zombie)->delay_frame = 0;
    }
    (*zombie)->delay_frame++;
}