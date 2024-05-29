#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../structures.h"
#include "../objects.h"
#include "../inventory.h"
#include "../characters.h"
#include "../Unity/src/unity.h"


void test_init_objects() {
    Object coffee = init_object(100,100,100,100,3,1,NULL); // café (vitesse) au sol
    TEST_ASSERT_EQUAL(100, coffee.x);
    TEST_ASSERT_EQUAL(100, coffee.y);
    TEST_ASSERT_EQUAL(100, coffee.width);
    TEST_ASSERT_EQUAL(100, coffee.height);
    TEST_ASSERT_EQUAL(3, coffee.type);
    TEST_ASSERT_EQUAL(1, coffee.ground);
    TEST_ASSERT_EQUAL(NULL, coffee.texture);
}


void test_use_object() {
    // Créer un inventaire
    Inventory* inventory = malloc(sizeof(Inventory));
    init_inventory(inventory);

    // Créer un personnage
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
        .inventory = *inventory
    };

    // Créer un objet "café"
    Object coffee = init_object(100,100,100,100,3,1,NULL);

    // Ajouter l'objet à l'inventaire du personnage
    add_to_inventory(&character.inventory, &coffee);

    // Vérifier que l'objet a été ajouté à l'inventaire
    TEST_ASSERT_EQUAL_INT(1, character.inventory.count);
    TEST_ASSERT_EQUAL_INT(3, character.inventory.items[0].type);

    // Utiliser l'objet
    int selected_item = 0;
    use_object(&character, &character.inventory.items[selected_item], selected_item);

    // Vérifier que l'objet a été supprimé de l'inventaire
    TEST_ASSERT_EQUAL_INT(0, character.inventory.count);

    // Vérifier que la vitesse du personnage a augmenté
    TEST_ASSERT_FLOAT_WITHIN(0.1, character.speed, 2.5);

    // Libérer la mémoire allouée
    free(inventory);
}


void test_check_object_collision() {
    // Créer un inventaire
    Inventory* inventory = malloc(sizeof(Inventory));
    init_inventory(inventory);

    // Créer un personnage
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
        .inventory = *inventory
    };

    // Créer un objet béret
    Object beret = init_object(100,100,100,100,2,1,NULL);

    // Vérifier qu'il n'y a pas de collision entre l'objet et le personnage
    TEST_ASSERT_EQUAL_INT(0, check_object_collision(&beret, &character));

    // Déplacer l'objet pour qu'il entre en collision avec le personnage
    beret.x = character.x + character.width / 2;
    beret.y = character.y + character.height / 2;

    // Vérifier qu'il y a une collision entre l'objet et le personnage
    TEST_ASSERT_EQUAL_INT(1, check_object_collision(&beret, &character));

    // Vérifier que l'objet a été ajouté à l'inventaire du personnage
    TEST_ASSERT_EQUAL_INT(1, character.inventory.count);
    TEST_ASSERT_EQUAL_INT(2, character.inventory.items[0].type);

    // Vérifier que l'objet n'est plus sur le sol
    TEST_ASSERT_EQUAL_INT(0, beret.ground);

    // Libérer la mémoire allouée
    free(inventory);
}


void test_generate_random_objects() {
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
    int tailleMapHoles = 10;
    int* tabMapHoles = malloc(tailleMapHoles * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        tabMapHoles[i] = i;
    }

    int numRandomObjects = rand() % MAX_RANDOM_OBJECTS;
    Object randomObjects[MAX_RANDOM_OBJECTS];
    generate_random_objects(tailleMapHoles, tabMapHoles, renderer, numRandomObjects, randomObjects);
    for (int i=0; i<numRandomObjects; i++) {
        TEST_ASSERT_TRUE(randomObjects[i].type >= 0 && randomObjects[i].type <= 3);
    }

    free(tabMapHoles);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
