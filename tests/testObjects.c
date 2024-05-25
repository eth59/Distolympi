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
    TEST_ASSERT_EQUAL(coffee.x, 100);
    TEST_ASSERT_EQUAL(coffee.y, 100);
    TEST_ASSERT_EQUAL(coffee.width, 100);
    TEST_ASSERT_EQUAL(coffee.height, 100);
    TEST_ASSERT_EQUAL(coffee.type, 3);
    TEST_ASSERT_EQUAL(coffee.ground, 1);
    TEST_ASSERT_EQUAL(coffee.texture, NULL);
}


void test_use_selected_objects_and_use_object() {
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
    
    Inventory* inventory;
    init_inventory(inventory);

    Object coffee = init_object(100,100,100,100,3,1,NULL); // café (vitesse) au sol
    add_to_inventory(inventory, &coffee);
    int selected_item = 1; // si l'item selectionné est le 1er de l'inventaire donc le café

    int initial_max_health = character.max_health;
    use_selected_object(&character,&selected_item);

    TEST_ASSERT_EQUAL(selected_item,-1); // café utilisé
    TEST_ASSERT_EQUAL(coffee.type,2); // bien de la vitesse
    TEST_ASSERT_EQUAL(character.max_health, initial_max_health+10); // effet du café sur le perso
}


void test_check_object_collision() {
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

    Object beret = init_object(100,100,100,100,2,1,NULL); // béret (défense) au sol
    int inventory_count = character.inventory.count;
    int res = check_object_collision(&beret,&character);
    TEST_ASSERT_EQUAL(res,1);
    TEST_ASSERT_EQUAL(character.inventory.count,inventory_count++);

    character.x = 300;
    character.y = 300;
    res = check_object_collision(&beret,&character);
    TEST_ASSERT_EQUAL(res,0);
}

void test_generate_random_objects() {
   // Object* randomObjects = generate_random_objects(int tailleMapHoles, int* tabMapHoles, SDL_Renderer* renderer, int numRandomObjects, Object* randomObjects) {
}
