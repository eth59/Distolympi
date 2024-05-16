#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "structures.h"
#include "characters.h"
#include "animations.h"
#include "selectRandomMap.h"
#include "collisionHandling.h"
#include "enemies.h"
#include "inventory.h"

#define MAX_ITEM_TYPES 3


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

    // création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }

    //SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

    // Initialisation du renderer
    SDL_Renderer *renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Texture*  dead_zombie_texture = get_texture("assets/dead_zombie.png",renderer);


    int running = 1;
    int in_menu = 1;
    int in_settings = 0;
    int selected_item = -1;

    while(running){
        int playing = 1;
        while(in_menu){
            while(in_settings){
                SDL_Event event;
                SDL_Texture* settings_texture = get_texture("assets/Settings.png",renderer);
                SDL_RenderCopy(renderer,settings_texture,NULL,NULL);
                SDL_RenderPresent(renderer);
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                            running = 0;
                            playing = 0;
                            in_menu = 0;
                            in_settings = 0;
                            break;
                        case SDL_KEYDOWN:
                            switch (event.key.keysym.sym) {
                                case SDLK_ESCAPE:
                                    in_settings = 0;
                                    break;
                            }
                            break;
                    }
                }
            }
            SDL_Event event;
            SDL_Texture* menu_texture = get_texture("assets/Menu.png",renderer);
            SDL_RenderCopy(renderer,menu_texture,NULL,NULL);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                        running = 0;
                        in_menu = 0;
                        playing = 0;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                in_menu = 0;
                                break;
                            case SDLK_ESCAPE:
                                in_settings = 1;
                                break;
                            case SDLK_RETURN:
                                in_menu = 0;
                                break;
                        }
                        break;
                }
            }
        }
        // Initialisation de l'inventaire
        Item* items = malloc(MAX_ITEM_TYPES * sizeof(Item));
        if (items == NULL) {
            printf("Erreur d'allocation de mémoire pour items\n");
            exit(1);
        }
        
        // selection aléatoire du fond d'écran
        int index = randomMapIndex();
        char *map = getMapFromIndex(index);
        
        // on récupère également la collisionTable
        char *collisionTableFileName = getColliderTable(index);
        char *collisionTable = openCollisionFile(collisionTableFileName);


        // charger fond d'écran
        SDL_Texture *backgroundTexture = get_texture(map, renderer);

        // liberer la mémoire allouée a map
        free(map);
        // J'ai bougé le free du collisionTableFileName à la fin parce que j'en ai besoin pour le zombie

        // Charger la texture des attaques
        SDL_Texture *attacksTexture = get_texture("assets/Attaquesx.png",renderer);
        // On divise la tileset du character 
        int attacks_max_column_frame=5;
        int attacks_max_line_frame=8;
        SDL_Rect* attacksRectsrc=get_frames(32,32,attacks_max_line_frame,attacks_max_column_frame);

        // Charger la texture du personnage
        SDL_Texture *characterTexture = get_texture("assets/loli.png",renderer);
        // Initialisation du personnage
        Character character = {
            .x = SCREEN_WIDTH / 2,
            .y = SCREEN_HEIGHT / 2,
            .width = SCREEN_WIDTH/16 ,
            .height =SCREEN_HEIGHT/9,
            .speed = 2,
            .hitBoxHeight = SCREEN_HEIGHT/36,
            .hitBoxWidth = SCREEN_WIDTH/32,
            .xHitBox = SCREEN_WIDTH/2 + SCREEN_WIDTH/64,
            .yHitBox = SCREEN_HEIGHT/2 + SCREEN_HEIGHT/9 - SCREEN_HEIGHT/36,
            .attack_speed = 0.5,
            .health = 100,
            .max_health = 100,
            .attack_damage = 10,
            .inventory = {0}
        };
        // On divise la tileset du character 
        int character_max_column_frame=8;
        int character_max_line_frame=10;
        SDL_Rect* characterRectsrc=get_frames(32,32,character_max_line_frame,character_max_column_frame);

        
        // Initialisation d'un 1er objet : un fromage
        Object cheese = init_object(50, 50, 100, 100, 0, 1);

        // Charger la texture du fromage
        char* cheese_path = "assets/cheese.png";
        load_item_textures(renderer, items, cheese_path, 0); // pour charger le fromage
        SDL_Texture *cheeseTexture = get_texture(cheese_path,renderer);

        // Initialisation du zombie
        Enemy *zombie;
        SDL_Texture *zombieTexture;
        SDL_Rect *zombieRectSrc;
        init_zombie(&zombie, renderer, &zombieTexture, &zombieRectSrc, 1);

        // Charger la texture de l'inventaire
        SDL_Texture* inventoryTexture = get_texture("assets/inventory.png", renderer);
        if (!inventoryTexture) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading inventory texture: %s", SDL_GetError());
            exit(-1);
        }
        
        SDL_Event event;
        // Déclaration de variables pour suivre le temps de mort
        int death_time = 0;
        // Déclaration de variables pour suivre l'état des touches
        int key_up_pressed = 0;
        int key_down_pressed = 0;
        int key_left_pressed = 0;
        int key_right_pressed = 0;
        int key_space_pressed=0;
        int mouseX = 0;
        int mouseY = 0;
        
        //attacks variables
        int attack_time;
        int attacks_animation_frame = 0;
        int attacks_delay_frame = 0;
        int attack_dispo = 1;
        int attack_flag = 0;

        int cheeseAdded = 0;

        // flags pour animation
        int direction = 0;
        int character_animation_frame = 0;//quel etape du cycle d'animation
        int character_delay_frame = 0;//compteur pour ne pas actualiser a chaque rendu
        //
        int low_on_life_time;
        int low_on_life_frame = 0;
        SDL_Texture* low_on_life_texture = get_texture("assets/Low_life1.png",renderer);
        int last_zombie_hit = -1000 ;
        int game_in_pause = 0;
        while (playing) {
            startTime = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                        running = 0;
                        playing = 0;
                        break;
                    case SDL_MOUSEMOTION:
                        // Récupération des coordonnées de la souris
                        mouseX = event.motion.x;
                        mouseY = event.motion.y;
                        break;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            // Pour chaque touche de déplacement on met le booléen
                            // correspondant à 1 quand la touche est pressée
                            case SDLK_ESCAPE:
                                game_in_pause = 1;
                                key_left_pressed = 0;
                                key_right_pressed = 0;
                                key_up_pressed = 0;
                                key_down_pressed = 0;
                                break;
                            case SDLK_r:
                                playing = 0;
                                break;
                            case SDLK_e:
                                // Vérifier si les coordonnées du personnage se trouvent dans la zone du fromage avec une marge de tolérance
                                if (!cheeseAdded && character.x + character.width >= cheese.x && character.x <= cheese.x + cheese.width &&
                                    character.y + character.height >= cheese.y && character.y <= cheese.y + cheese.height) {
                                    add_to_inventory(&character.inventory, cheese);
                                    cheese.ground = 0; 
                                    SDL_DestroyTexture(cheeseTexture);
                                    cheeseAdded = 1;
                                }
                                break;
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
                            case SDLK_SPACE:
                                key_space_pressed = 1;
                                break;
                            case SDLK_1:
                                selected_item = 0;
                                use_object(&character, &character.inventory.items[selected_item], selected_item);
                                break;
                            case SDLK_2: 
                                selected_item = 1;
                                use_object(&character, &character.inventory.items[selected_item], selected_item);
                                break;
                            case SDLK_3: 
                                selected_item = 2;
                                use_object(&character, &character.inventory.items[selected_item], selected_item);
                                break;
                            case SDLK_4:
                                selected_item = 3;
                                use_object(&character, &character.inventory.items[selected_item], selected_item);
                                break;
                            case SDLK_5:
                                selected_item = 4;
                                use_object(&character, &character.inventory.items[selected_item], selected_item);
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
                            case SDLK_SPACE:
                                key_space_pressed = 0;
                                break;
                        }
                        break;
                }
            }

            if (key_up_pressed || key_down_pressed || key_left_pressed || key_right_pressed) {
                direction=get_direction_and_move(key_up_pressed,key_down_pressed,key_left_pressed,key_right_pressed,&character,displayMode, collisionTable);
            }

           // Mettre à jour la variable selected_item en fonction des touches appuyées
            if (event.key.keysym.sym == SDLK_1) {
                selected_item = 0;
            } else if (event.key.keysym.sym == SDLK_2) { 
                selected_item = 1;
            } else if (event.key.keysym.sym == SDLK_3) { 
                selected_item = 2;
            } else if (event.key.keysym.sym == SDLK_4) {
                selected_item = 3;
            } else if (event.key.keysym.sym == SDLK_5) {
                selected_item = 4;
            } else {
                // Réinitialiser la variable selected_item si aucune touche d'inventaire n'est appuyée
                selected_item = -1;
            }

            // rendu graphique
            SDL_RenderClear(renderer);

            int max_column_frame = 8;
            int max_line_frame = 10;

            // Dessiner le fond
            SDL_Rect backgroundRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT-120};
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

            if(zombie->health>0){
                // Actions mobs & gestion interaction
                pathfinding(zombie, &character, collisionTableFileName);
                move_zombie(&zombie, &character);
                // Rendu du zombie
                render_zombie(zombie, renderer, zombieTexture, zombieRectSrc);
            }
            SDL_Rect zombieRectDest = {(int)zombie->x, (int)zombie->y, zombie->width, zombie->height};

            //Rendu attacks 
            if (SDL_GetTicks()-attack_time>=1000/character.attack_speed){
                attack_dispo = 1;
            }

            if ((key_space_pressed && attack_dispo) | attack_flag){
                attack_flag = 1;
                attack_time=SDL_GetTicks();
                int direction_attack = get_melee_direction(character.x + character.width/2,character.y + character.height/2,mouseX,mouseY);
                SDL_Rect attacksRectdest = get_Rectdest_attacks(direction_attack,character);
    
                attacks_animation_frame=attacks_animation_frame%attacks_max_column_frame;// cycle d'animation
                SDL_RenderCopy(renderer, attacksTexture, &attacksRectsrc[(direction_attack)*attacks_max_column_frame+attacks_animation_frame], &attacksRectdest);
                if(attacks_delay_frame>5){
                        attacks_animation_frame++;
                        attacks_delay_frame=0;}    
                if(attacks_animation_frame>4){
                    attack_dispo = 0;
                    attack_flag = 0;
                    // L'attaque se fait si le zombie est à portée et plus si collision entre les hitboxs pour un meilleur rendu
                    if (zombie_is_in_range(zombie, &character)) {
                        zombie->health = zombie->health - character.attack_damage;
                        printf("HIT! Zombie's life is now %d\n",zombie->health);
                    }
                }
                attacks_delay_frame++;
            }
                
            
            //Rendu character + animation 
            int key_pressed=key_down_pressed | key_left_pressed | key_right_pressed | key_up_pressed;//On regarde si une touche de direction est appuyé
            character_animation_frame=character_animation_frame%character_max_column_frame;// cycle d'animation
            SDL_Rect characterRectdest = {(int)character.x, (int)character.y, (int)character.width, (int)character.height};
            SDL_Rect characterHitboxRect = {character.xHitBox, character.y, character.hitBoxWidth, character.height};
            SDL_Rect zombieHitboxRect = {zombie->x + SCREEN_WIDTH/64, zombie->y, zombie->width - SCREEN_WIDTH/32, zombie->height};
            
            
            if(character_delay_frame>20/character.speed){
                character_animation_frame++;
                character_delay_frame=0;
            }
            character_delay_frame++;

            if (zombie_is_in_range(zombie, &character) && zombie->health>0 && character.health>0) {
                zombie->isInsidePlayer = 1;
                if (SDL_GetTicks()-last_zombie_hit>=500){
                    last_zombie_hit = SDL_GetTicks();
                    zombie_attack(zombie, &character, renderer);
                }
            } else {
                zombie->isInsidePlayer = 0;
            }

            // Rendu du fromage
            SDL_Rect cheeseRect = {(int)cheese.x, (int)cheese.y, cheese.width, cheese.height};
            SDL_RenderCopy(renderer, cheeseTexture, NULL, &cheeseRect);

            if(character.health<=20){
                if( startTime-low_on_life_time>200){
                low_on_life_time = startTime;
                low_on_life_frame = 1 - low_on_life_frame;
                low_on_life_texture = get_texture("assets/Low_life1.png",renderer);
                if (low_on_life_frame == 1){low_on_life_texture = get_texture("assets/Low_life2.png",renderer);}}
                SDL_RenderCopy(renderer,low_on_life_texture,NULL,&backgroundRect);
            }

            if(character.health<=0){
                if(death_time==0){
                    death_time = SDL_GetTicks();
                }
                SDL_Texture* death_menu_texture=get_texture("assets/Wasted.png",renderer);
                SDL_RenderCopy(renderer,death_menu_texture,NULL,NULL);
                SDL_DestroyTexture(characterTexture);
                if(SDL_GetTicks()-death_time>3000){
                    playing = 0;
                    in_menu = 1;
                }
            }
            else if(zombie->health <= 0 ){
                zombie->speed = 0;
                SDL_RenderCopy(renderer,dead_zombie_texture,NULL,&zombieRectDest);
            }

            // On dessine le personnage tout a la fin pour que la texture soit au dessus de toutes les autres
            SDL_RenderCopy(renderer, characterTexture, &characterRectsrc[direction*character_max_column_frame+character_animation_frame*key_pressed], &characterRectdest);
            
            // Dessin de l'inventaire
            draw_inventory_bar(renderer, character, inventoryTexture, items);

            // Dessin de la barre de vie
            draw_health_bar(renderer, SCREEN_WIDTH-820, SCREEN_HEIGHT - SCREEN_HEIGHT/9 + 10, character.health, character.max_health);

            SDL_RenderPresent(renderer);
            
            // Cap the frame rate
            endTime = SDL_GetTicks();
            deltaTime = endTime - startTime;
            if (deltaTime < 1000 / FPS) {
                SDL_Delay((1000 / FPS) - deltaTime);
                if(game_in_pause){
                    SDL_Texture* game_in_pause_texture = get_texture("assets/Game-in-pause.png",renderer);
                    SDL_RenderCopy(renderer,game_in_pause_texture,NULL,&backgroundRect);
                    
                    SDL_RenderPresent(renderer);
                }
                while(game_in_pause){
                    if (SDL_PollEvent(&event)) {
                        switch (event.type) {
                            case SDL_QUIT:
                                // pour quitter le programme quand par exemple on appuie sur la croix de la fenêtre
                                running = 0;
                                playing = 0;
                                game_in_pause = 0;
                                break;
                            case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                    case SDLK_ESCAPE:
                                        game_in_pause = 0;
                                        playing = 0;
                                        in_menu = 1;
                                        break;
                                    case SDLK_SPACE:
                                        game_in_pause = 0;
                                        break;
                                }
                        }
                    }
                }
            }
        }
        
        // Libérer la mémoire et quitter SDL


        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyTexture(characterTexture);
        SDL_DestroyTexture(cheeseTexture);
        SDL_DestroyTexture(zombieTexture);
        SDL_DestroyTexture(inventoryTexture);

        free(characterRectsrc);
        free(attacksRectsrc);
        free(collisionTable);
        free(zombieRectSrc);
        free_zombie(zombie);
        free(collisionTableFileName);
        free(items);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("Stopped the game.\n");

    return 0;
}
