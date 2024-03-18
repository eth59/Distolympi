#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    float x;
    float y;
    float size;
} Character ;

void moveCharacter(Character *character, float dx, float dy) {
    character->x += dx;
    character->y += dy;
}

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
    SDL_Window *window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_MAXIMIZED);
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
    SDL_Surface *backgroundSurface = IMG_Load("map.png");
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
    SDL_Surface *characterSurface = IMG_Load("perso.png");
    if (!characterSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading character texture: %s", IMG_GetError());
        exit(-1);
    }

    SDL_Texture *characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface); // Libérer la surface après avoir créé la texture
    if (!characterTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating character texture: %s", SDL_GetError());
        exit(-1);
    }

    // initialisation du personnage
    Character character = {.x = displayMode.w / 2, .y = displayMode.h / 2, .size = 150};

    // boucle principale
    SDL_Event event;
    int running = 1;

    while (running) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            moveCharacter(&character, -10, 0);
                            break;
                        case SDLK_d:
                            moveCharacter(&character, 10, 0);
                            break;
                        case SDLK_z:
                            moveCharacter(&character, 0, -10);
                            break;
                        case SDLK_s:
                            moveCharacter(&character, 0, 10);
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        // actions mobs & gestion interaction - TO DO

        // changement états du jeu - TO DO

        // rendu graphique
        SDL_RenderClear(renderer);

        // Dessiner le fond
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        // Dessiner le personnage
        SDL_Rect characterRect = {(int)character.x, (int)character.y, (int)character.size, (int)character.size};
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);

        SDL_RenderPresent(renderer);
    }

    return 0;
}
