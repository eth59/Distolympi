#include <SDL2/SDL.h>
#include "structures.h"
#include "characters.h"


void moveCharacter(Character *character, Wall *wall, SDL_DisplayMode displayMode, float dx, float dy) {
    /*
    QUAND ON VEUT LA HAUTEUR DU PERSONNAGE IL FAUT FAIRE *1.4
    Pouquoi ?
    Parce que je ne sais pas.
    La hauteur est la bonne (sinon l'affichage du personnage serait déformé, disproportionné)
    Mais si on ne fait pas de multiplication, approximativement la moitié du perso sort de l'écran
    *1.4 c'est la valeur parfait pour arriver au bord de l'écran
    C et la SDL ou plutôt les trucs incompréhensibles 
    */

    // On commence par calculer la valeur des bordures
    int border_left = wall->left; // 0 sans mur
    int border_up = wall-> up; // 0 sans mur
    int border_right = displayMode.w - character->width - wall->right; // on soustrait la largeur du perso pour pas qu'il sorte de l'écran
    int border_down = displayMode.h - 1.4 * character->height - wall->down; // on soustrait la hauteur du perso pour pas qu'il sorte de l'écran (avec le fameux 1.4)

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