#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structures.h"
#include "characters.h"
#include "animations.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 240

int main() {
    Uint32 startTime, endTime, deltaTime;

    // Initialisation SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init: %s", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    // Initialisation de la fenêtre
    // Obtention des informations d'affichage
    SDL_DisplayMode displayMode;
    if (SDL_GetDesktopDisplayMode(0, &displayMode)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error getting display mode: %s", SDL_GetError());
        exit(-1);
    }

    // création de la fenêtre en mode plein écran fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }

    // Initialisation du renderer
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }

    // charger fond d'écran
    SDL_Texture *backgroundTexture = get_texture("assets/map.png",renderer);
    // Charger la texture du personnage
    SDL_Texture *characterTexture = get_texture("assets/loli.png",renderer);

    // Initialisation du personnage
    Character character = {
        .x = displayMode.w / 2,
        .y = displayMode.h / 2,
        .width = displayMode.w/16 ,
        .height =displayMode.h/9,
        .speed = 2
    };
    
    

    // On divise la tileset du character 
    int max_column_frame=8;
    int max_line_frame=10;
    SDL_Rect* characterRectsrc=get_frames(32,32,max_line_frame,max_column_frame);

    // Initialisation des murs (juste leurs tailles)
    Wall wall = {
        .left = 0,
        .up = 30,
        .right = -50,
        .down = 110
    };

    // Initialisation d'un 1er objet : un fromage
    Object cheese = {
        .x = 50,
        .y = 50,
        .height = 100,
        .width = 100,
        .ground = 1,
        .type = 0
    };

    // Charger la texture du fromage
    SDL_Texture *cheeseTexture = get_texture("assets/cheese.png",renderer);

    // Boucle principale
    SDL_Event event;
    int running = 1;

    // Déclaration de variables pour suivre l'état des touches
    int key_up_pressed = 0;
    int key_down_pressed = 0;
    int key_left_pressed = 0;
    int key_right_pressed = 0;
    // flags pour animation
    int direction=0;
    int animation_frame=0;//quel etape du cycle d'animation
    int delay_frame=0;//compteur pour ne pas actualiser a chaque rendu
    while (running) {
        startTime = SDL_GetTicks();

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                    running=0;
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
            }
        }

        if (key_up_pressed || key_down_pressed || key_left_pressed || key_right_pressed) {
            direction=get_direction_and_move(key_up_pressed,key_down_pressed,key_left_pressed,key_right_pressed,&character,&wall,displayMode);
        }

        // Actions mobs & gestion interaction

        // Vérifier si les coordonnées du personnage se trouvent dans la zone du fromage avec une marge de tolérance
        if (character.x + character.width >= cheese.x && character.x <= cheese.x + cheese.width &&
            character.y + character.height >= cheese.y && character.y <= cheese.y + cheese.height) {
            cheese.ground = 0; // le fromage est dans l'inventaire maintenant
            SDL_DestroyTexture(cheeseTexture);
        }

        // Changement états du jeu - TO DO

        // rendu graphique
        SDL_RenderClear(renderer);

        // Dessiner le fond
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        int key_pressed=key_down_pressed | key_left_pressed | key_right_pressed | key_up_pressed;//On regarde si une touche de direction est appuyé
        animation_frame=animation_frame%max_column_frame;// cycle d'animation
        SDL_Rect characterRectdest = {(int)character.x, (int)character.y, (int)character.width, (int)character.height};
        SDL_RenderCopy(renderer, characterTexture, &characterRectsrc[direction*max_column_frame+animation_frame*key_pressed], &characterRectdest);

        // Rendu du fromage
        SDL_Rect cheeseRect = {(int)cheese.x, (int)cheese.y, cheese.width, cheese.height};
        SDL_RenderCopy(renderer, cheeseTexture, NULL, &cheeseRect);

        SDL_RenderPresent(renderer);
        if(delay_frame>20/character.speed){
            animation_frame++;
            delay_frame=0;
        }
        delay_frame++;

        // Cap the frame rate
        endTime = SDL_GetTicks();
        deltaTime = endTime - startTime;
        if (deltaTime < 1000 / FPS) {
            SDL_Delay((1000 / FPS) - deltaTime);
        }
    }
    // Libérer la mémoire et quitter SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(cheeseTexture);
    SDL_Quit();
    printf("stoped the game\n");
    exit(-1);
    return 0;
}
