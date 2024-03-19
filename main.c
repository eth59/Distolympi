#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structures.h"
#include "characters.h"


int main() {
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    // init fenetre
    // obtenir les informations d'affichage
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting display mode: %s", SDL_GetError());
        exit(-1);
    }

    // création de la fenêtre en mode plein écran fenêtre
    SDL_Window *window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }

    // init renderer
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }

    // charger fond d'écran
    SDL_Surface *backgroundSurface = IMG_Load("assets/map.png");
    if (!backgroundSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading background: %s", IMG_GetError());
        exit(-1);
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating background texture: %s", SDL_GetError());
        exit(-1);
    }

    // Charger la texture du personnage
    SDL_Surface *characterSurface = IMG_Load("assets/fleches.png");
    if (!characterSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading character texture: %s", IMG_GetError());
        exit(-1);
    }

    // initialisation du personnage
    Character character = {
        .x = displayMode.w / 2,
        .y = displayMode.h / 2,
        .width = 200,
        .height = 200, // le nombre ici est le même que celui juste au dessus (c'est un produit en croix)
        .speed = 5.0
    };

    SDL_Texture *characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface); // Libérer la surface après avoir créé la texture
    if (!characterTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating character texture: %s", SDL_GetError());
        exit(-1);
    }

    // Initialisation des murs (juste leurs tailles)
    Wall wall = {
        .left = 0,
        .up = 30,
        .right = 0,
        .down = 90
    };

    // boucle principale
    SDL_Event event;
    int running = 1;

    // Declare global flags to track key states
    int key_up_pressed = 0;
    int key_down_pressed = 0;
    int key_left_pressed = 0;
    int key_right_pressed = 0;
    int direction=0;
    while (running) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                    exit(-1);
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        // Pour chaque touche de déplacement on met le booléen
                        // correspondant à 1 quand la touche est pressée
                        case SDLK_q:
                            key_left_pressed = 1;
                            break;
                        case SDLK_d:
                            key_right_pressed = 1;
                            break;
                        case SDLK_z:
                            key_up_pressed = 1;
                            break;
                        case SDLK_s:
                            key_down_pressed = 1;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        // Pour chaque touche de déplacement on met le booléen
                        // correspondant à 0 quand la touche est relachée
                        case SDLK_q:
                            key_left_pressed = 0;
                            break;
                        case SDLK_d:
                            key_right_pressed = 0;
                            break;
                        case SDLK_z:
                            key_up_pressed = 0;
                            break;
                        case SDLK_s:
                            key_down_pressed = 0;
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                // Gestion des événements de fenêtre
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // La fenêtre a été redimensionnée, ajustons la position et la taille du personnage
                    float characterXRatio = (float)character.x / displayMode.w;
                    float characterYRatio = (float)character.y / displayMode.h;
                    float characterWidthRatio = (float)character.width / displayMode.w;
                    float characterHeightRatio = (float)character.height / displayMode.h;
                    displayMode.w = event.window.data1; // Nouvelle largeur de la fenêtre
                    displayMode.h = event.window.data2; // Nouvelle hauteur de la fenêtre
                    character.x = characterXRatio * displayMode.w;
                    character.y = characterYRatio * displayMode.h;
                    character.width = characterWidthRatio * displayMode.w;
                    character.height = characterHeightRatio * displayMode.h;
                }
                break;
            }
        }

        // Déplacement du personnage
        // On multiplie par sqrt(2) en diagonale
        // pour éviter une impression de vitesse plus élévée
        if (key_left_pressed && key_up_pressed) {
            direction=0;
           moveCharacter(&character, &wall, displayMode, -character.speed*0.7071f, -character.speed*0.7071f);
        } else if (key_left_pressed && key_down_pressed) {
            direction=0;
           moveCharacter(&character, &wall, displayMode, -character.speed*0.7071f, character.speed*0.7071f);
        } else if (key_right_pressed && key_up_pressed) {
            direction=1;
            moveCharacter(&character, &wall, displayMode, character.speed*0.7071f, -character.speed*0.7071f);
        } else if (key_right_pressed && key_down_pressed) {
            direction=1;
            moveCharacter(&character, &wall, displayMode, character.speed*0.7071f, character.speed*0.7071f);
        } else if (key_left_pressed) {
            direction=0;
            moveCharacter(&character, &wall, displayMode, -character.speed, 0);
        } else if (key_right_pressed) {
            direction=1;
            moveCharacter(&character, &wall, displayMode, character.speed, 0);
        } else if (key_up_pressed) {
            direction=3;
            moveCharacter(&character, &wall, displayMode, 0, -character.speed);
        } else if (key_down_pressed) {
            direction=2;
            moveCharacter(&character, &wall, displayMode, 0, character.speed);
        }

        // actions mobs & gestion interaction - TO DO

        // changement états du jeu - TO DO

        // rendu graphique
        SDL_RenderClear(renderer);

        // Dessiner le fond
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        // modif ici nino
        SDL_Rect characterRectsrc[4];
        int pointeur_frame=0;
        int i;
        int u;
        int v;
        for(i=0;i<4;i++){
            u=0;
            v=16*i;
            characterRectsrc[i].x=u;
            characterRectsrc[i].y=v;
            characterRectsrc[i].w=16;
            characterRectsrc[i].h=16;
        }
        SDL_Rect characterRectdest = {(int)character.x, (int)character.y, (int)character.width, (int)character.height};
        SDL_RenderCopy(renderer, characterTexture, &characterRectsrc[direction], &characterRectdest);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
