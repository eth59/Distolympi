#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Unity/src/unity.h"
#include "../animations.h"
#include "../structures.h"
#include "../characters.h"
#include "../enemies.h"

void test_init_zombie()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }

    // Zombie au corps à corps
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    TEST_ASSERT(zombie->x >= 0 && zombie->x <= SCREEN_WIDTH);
    TEST_ASSERT(zombie->y >= 0 && zombie->y < SCREEN_HEIGHT);
    TEST_ASSERT_EQUAL(zombie->width, SCREEN_WIDTH / 16);
    TEST_ASSERT_EQUAL(zombie->height, SCREEN_HEIGHT / 9);
    TEST_ASSERT_EQUAL(zombie->speed, 1.5);
    TEST_ASSERT_EQUAL(zombie->attack_damage, 5);
    TEST_ASSERT_EQUAL(zombie->health, 50);
    TEST_ASSERT_EQUAL(zombie->range, 50);
    TEST_ASSERT_EQUAL(zombie->isAShooter, 0);

    // Zombie tireur
    Enemy* zombie2 = malloc(sizeof(Enemy));
    init_zombie(&zombie2, renderer, 1, 10, tabMapHoles, 1, 2);
    TEST_ASSERT(zombie2->x >= 0 && zombie2->x <= SCREEN_WIDTH);
    TEST_ASSERT(zombie2->y >= 0 && zombie2->y < SCREEN_HEIGHT);
    TEST_ASSERT_EQUAL(zombie2->width, SCREEN_WIDTH / 16);
    TEST_ASSERT_EQUAL(zombie2->height, SCREEN_HEIGHT / 9);
    TEST_ASSERT_EQUAL(zombie2->speed, 1.5);
    TEST_ASSERT_EQUAL(zombie2->attack_damage, 10);
    TEST_ASSERT_EQUAL(zombie2->health, 30);
    TEST_ASSERT_EQUAL(zombie2->range, 300);
    TEST_ASSERT_EQUAL(zombie2->isAShooter, 1);

    // Free Zone
    free_zombie(zombie);
    free_zombie(zombie2);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_add_medal()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // Création d'une première médaille (de bronze)
    Medal medal;
    medal.x = zombie->x + zombie->width/2; // La médaille apparaît au milieu du zombie
    medal.y = zombie->y + zombie->height/2; // La médaille apparaît au milieu du zombie
    medal.width = 32;
    medal.height = 32;
    medal.speed = 2;
    medal.angle = 0;
    get_texture(&medal.texture, "assets/bronze_medal.png", renderer);

    // Ajout de la médaille au zombie
    addMedal(zombie, medal);
    TEST_ASSERT_EQUAL(zombie->medals->medal.x, zombie->x + zombie->width / 2);
    TEST_ASSERT_EQUAL(zombie->medals->medal.y, zombie->y + zombie->height / 2);
    TEST_ASSERT_EQUAL(zombie->medals->medal.width, 32);
    TEST_ASSERT_EQUAL(zombie->medals->medal.height, 32);
    TEST_ASSERT_EQUAL(zombie->medals->medal.speed, 2);
    TEST_ASSERT_EQUAL(zombie->medals->medal.angle, 0);

    // Création d'une deuxième médaille (d'argent)
    Medal medal2;
    medal2.x = zombie->x + zombie->width/2; // La médaille apparaît au milieu du zombie
    medal2.y = zombie->y + zombie->height/2; // La médaille apparaît au milieu du zombie
    medal2.width = 32;
    medal2.height = 32;
    medal2.speed = 2;
    medal2.angle = 0;
    get_texture(&medal2.texture, "assets/silver_medal.png", renderer);

    // Ajout de la médaille au zombie
    addMedal(zombie, medal2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.x, zombie->x + zombie->width / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.y, zombie->y + zombie->height / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.width, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.height, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.speed, 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.angle, 0);

    // Création d'une troisième médaille (d'or)
    Medal medal3;
    medal3.x = zombie->x + zombie->width/2; // La médaille apparaît au milieu du zombie
    medal3.y = zombie->y + zombie->height/2; // La médaille apparaît au milieu du zombie
    medal3.width = 32;
    medal3.height = 32;
    medal3.speed = 2;
    medal3.angle = 0;
    get_texture(&medal3.texture, "assets/silver_medal.png", renderer);

    // Ajout de la médaille au zombie
    addMedal(zombie, medal3);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.x, zombie->x + zombie->width / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.y, zombie->y + zombie->height / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.width, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.height, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.speed, 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->next->medal.angle, 0);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_init_medal()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // Initialisation d'une médaille
    init_medal(zombie, &character, renderer);
    TEST_ASSERT_EQUAL(zombie->medals->medal.x, zombie->x + zombie->width / 2);
    TEST_ASSERT_EQUAL(zombie->medals->medal.y, zombie->y + zombie->height / 2);
    TEST_ASSERT_EQUAL(zombie->medals->medal.width, 32);
    TEST_ASSERT_EQUAL(zombie->medals->medal.height, 32);
    TEST_ASSERT_EQUAL(zombie->medals->medal.speed, 2);

    // Initialisation d'une deuxième médaille
    init_medal(zombie, &character, renderer);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.x, zombie->x + zombie->width / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.y, zombie->y + zombie->height / 2);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.width, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.height, 32);
    TEST_ASSERT_EQUAL(zombie->medals->next->medal.speed, 2);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_remove_medal()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // Initialisation d'une médaille
    init_medal(zombie, &character, renderer);

    // On retire la médaille
    removeMedal(zombie, zombie->medals->medal);
    TEST_ASSERT_EQUAL(zombie->medals, NULL);

    // On initialise 2 médailles
    init_medal(zombie, &character, renderer);
    init_medal(zombie, &character, renderer);

    // On retire la première médaille
    removeMedal(zombie, zombie->medals->medal);
    int nb_medals = 0;
    MedalList* current = zombie->medals;
    while (current != NULL)
    {
        nb_medals++;
        current = current->next;
    }
    TEST_ASSERT_EQUAL(nb_medals, 1);

    // On retire la deuxième médaille
    removeMedal(zombie, zombie->medals->medal);
    TEST_ASSERT_EQUAL(zombie->medals, NULL);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_zombie_is_in_range()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // Le joueur est à portée
    character.x = zombie->x + zombie->range / 2;
    character.y = zombie->y;
    TEST_ASSERT_EQUAL(zombie_is_in_range(zombie, &character), 1);

    // Le joueur n'est pas à portée
    character.x = zombie->x + zombie->range + 1;
    character.y = zombie->y;
    TEST_ASSERT_EQUAL(zombie_is_in_range(zombie, &character), 0);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_calculate_shooting_angle()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // Le joueur est à droite du zombie
    character.x = zombie->x + zombie->range / 2;
    character.y = zombie->y;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), 0);

    // Le joueur est en bas à droite du zombie
    character.x = zombie->x + zombie->range / 2;
    character.y = zombie->y + zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), PI / 4);

    // Le joueur est en bas du zombie
    character.x = zombie->x;
    character.y = zombie->y + zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), PI / 2);

    // Le joueur est en bas à gauche du zombie
    character.x = zombie->x - zombie->range / 2;
    character.y = zombie->y + zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), 3 * PI / 4);

    // Le joueur est à gauche du zombie
    character.x = zombie->x - zombie->range / 2;
    character.y = zombie->y;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), PI);

    // Le joueur est en haut à gauche du zombie
    character.x = zombie->x - zombie->range / 2;
    character.y = zombie->y - zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), -3 * PI / 4);

    // Le joueur est en haut du zombie
    character.x = zombie->x;
    character.y = zombie->y - zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), -PI / 2);

    // Le joueur est en haut à droite du zombie
    character.x = zombie->x + zombie->range / 2;
    character.y = zombie->y - zombie->range / 2;
    TEST_ASSERT_EQUAL(calculateShootingAngle(zombie, &character), -PI / 4);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_zombie_attack()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie au corps à corps
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un zombie à distance
    Enemy* zombie2 = malloc(sizeof(Enemy));
    init_zombie(&zombie2, renderer, 1, 10, tabMapHoles, 1, 2);
    // Initialisation d'un joueur
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

    // Le zombie au corps à corps attaque le joueur
    zombie_attack(zombie, &character, renderer);
    TEST_ASSERT_EQUAL(character.health, 95);

    // Le zombie tireur attaque le joueur
    zombie_attack(zombie2, &character, renderer);
    TEST_ASSERT_EQUAL(zombie2->medals->medal.x, zombie2->x + zombie2->width / 2); // On veut juste vérifier qu'une médaille a été créée

    // Free Zone
    free_zombie(zombie);
    free_zombie(zombie2);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_medal_touch_player()
{
    // Initialisation d'un joueur
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

    // Initialisation d'une médaille
    Medal medal;
    medal.x = character.x;
    medal.y = character.y;
    medal.width = 32;
    medal.height = 32;
    medal.speed = 2;
    medal.angle = 0;

    // La médaille touche le joueur
    TEST_ASSERT_EQUAL(medalTouchPlayer(medal, &character), 1);

    // La médaille ne touche pas le joueur
    medal.x = character.x + character.width + 1;
    TEST_ASSERT_EQUAL(medalTouchPlayer(medal, &character), 0);
}

void test_move_zombie()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
    Character character = {
        .x = 100,
        .y = 100,
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

    // Le zombie est à droite du joueur
    zombie->x = character.x + zombie->range + 1;
    zombie->y = character.y;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x + zombie->range + 1 - zombie->speed, zombie->x);

    // Le zombie est en bas à droite du joueur
    zombie->x = character.x + zombie->range + 1;
    zombie->y = character.y + zombie->range + 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x + zombie->range + 1 - zombie->speed * 0.7071f, zombie->x);
    TEST_ASSERT_EQUAL(character.y + zombie->range + 1 - zombie->speed * 0.7071f, zombie->y);

    // Le zombie est en bas du joueur
    zombie->x = character.x;
    zombie->y = character.y + zombie->range + 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.y + zombie->range + 1 - zombie->speed, zombie->y);

    // Le zombie est en bas à gauche du joueur
    zombie->x = character.x - zombie->range - 1;
    zombie->y = character.y + zombie->range + 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x - zombie->range - 1 + zombie->speed * 0.7071f, zombie->x);
    TEST_ASSERT_EQUAL(character.y + zombie->range + 1 - zombie->speed * 0.7071f, zombie->y);

    // Le zombie est à gauche du joueur
    zombie->x = character.x - zombie->range - 1;
    zombie->y = character.y;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x - zombie->range - 1 + zombie->speed, zombie->x);

    // Le zombie est en haut à gauche du joueur
    zombie->x = character.x - zombie->range - 1;
    zombie->y = character.y - zombie->range - 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x - zombie->range - 1 + zombie->speed * 0.7071f, zombie->x);

    // Le zombie est en haut du joueur
    zombie->x = character.x;
    zombie->y = character.y - zombie->range - 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.y - zombie->range - 1 + zombie->speed, zombie->y);

    // Le zombie est en haut à droite du joueur
    zombie->x = character.x + zombie->range + 1;
    zombie->y = character.y - zombie->range - 1;
    move_zombie(&zombie, &character);
    TEST_ASSERT_EQUAL(character.x + zombie->range + 1 - zombie->speed * 0.7071f, zombie->x);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_read_map_collision_file()
{
    int** map = readMapCollisionFile("assets/map01.txt");
    TEST_ASSERT_EQUAL(1, map[0][0]);
    TEST_ASSERT_EQUAL(1, map[0][7]);
    TEST_ASSERT_EQUAL(0, map[1][1]);

    // Free Zone
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        free(map[i]);
    }
    free(map);
}

void test_map_to_graph()
{
    int** map = readMapCollisionFile("assets/map01.txt");
    float** graph = mapToGraph(map);
    TEST_ASSERT_EQUAL(0, graph[0][0]);
    TEST_ASSERT_EQUAL(1, graph[0][1]);

    // Free Zone
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        free(map[i]);
    }
    free(map);
    for (int i = 0; i < V; i++)
    {
        free(graph[i]);
    }
    free(graph);
}

void test_min_distance()
{
    float dist[V];
    for (int i = 0; i < V; i++)
    {
        dist[i] = 5;
    }
    
    int vu[V] = {0};
    dist[0] = 0;
    dist[1] = 1;
    dist[2] = 2;
    vu[0] = 0;
    vu[1] = 1;
    vu[2] = 0;
    TEST_ASSERT_EQUAL(0, minDistance(dist, vu));
}

void test_dijkstra()
{
    int** map = readMapCollisionFile("assets/map01.txt");
    float** graph = mapToGraph(map);
    TEST_ASSERT_EQUAL(1, dijkstra(graph, 0, 3));

    // Free Zone
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        free(map[i]);
    }
    free(map);
    for (int i = 0; i < V; i++)
    {
        free(graph[i]);
    }
    free(graph);
}

void test_get_sommet()
{
    int x = 0;
    int y = 0;
    int hitboxWidth = 10;
    int hitboxHeight = 10;
    TEST_ASSERT_EQUAL(0, get_sommet(x, y, hitboxWidth, hitboxHeight));
    x = 1000;
    y = 500;
    hitboxWidth = 100;
    hitboxHeight = 100;
    TEST_ASSERT_EQUAL(49, get_sommet(x, y, hitboxWidth, hitboxHeight));
}

void test_pathfinding()
{
    // Initialisation d'un renderer et d'une fenêtre
    SDL_Window *window = SDL_CreateWindow("LE JEU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in window init: %s", SDL_GetError());
        exit(-1);
    }
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in renderer init: %s", SDL_GetError());
        exit(-1);
    }
    // Initialisation d'un tableau dont la fonction a besoin
    int* tabMapHoles = malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }
    // Initialisation d'un zombie
    Enemy* zombie = malloc(sizeof(Enemy));
    init_zombie(&zombie, renderer, 0, 10, tabMapHoles, 0, 2);
    // Initialisation d'un joueur
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

    // On change la position du zombie pour les tests
    zombie->x = 100;
    zombie->y = 100;

    pathfinding(zombie, &character, "assets/map01.txt");
    TEST_ASSERT_EQUAL(1, zombie->nextVertex);

    // Free Zone
    free_zombie(zombie);
    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}