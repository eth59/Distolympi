#include "../Unity/src/unity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "../structures.h"
#include "../characters.h"
#include "testCharacter.h"

void test_moveCharacter() {
    Character character = {
            .x = 0,
            .y = 0,
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
    char *collisionTable = "01000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    SDL_DisplayMode displayMode = {0};
    moveCharacter(&character, displayMode, 0.5, 0.5, collisionTable);
    TEST_ASSERT_EQUAL_FLOAT(0.5, character.x);
    TEST_ASSERT_EQUAL_FLOAT(0.5, character.y);

    Character character2 = {
        .x = SCREEN_WIDTH/32,
        .y = 0,
        .width = SCREEN_WIDTH/16 ,
        .height =SCREEN_HEIGHT/9,
        .speed = 2,
        .hitBoxHeight = SCREEN_HEIGHT/36,
        .hitBoxWidth = SCREEN_WIDTH/32,
        .xHitBox = SCREEN_WIDTH/32 + SCREEN_WIDTH/64,
        .yHitBox = SCREEN_HEIGHT/9 - SCREEN_HEIGHT/36,
        .attack_speed = 0.5,
        .health = 100,
        .max_health = 100,
        .attack_damage = 10,
        .inventory = {0}
    };

    moveCharacter(&character2, displayMode, 0.5, 0.5, collisionTable);
    TEST_ASSERT_EQUAL_FLOAT(SCREEN_WIDTH/32, character2.x);
    TEST_ASSERT_EQUAL_FLOAT(0.5, character2.y);
}

// Test cases for get_direction_and_move function
void test_get_direction_and_move(void) {
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
    SDL_DisplayMode displayMode = {0};
    char collisionTable[100] = {0};
    
    int direction = get_direction_and_move(1, 0, 1, 0, &character, displayMode, collisionTable);
    TEST_ASSERT_EQUAL_INT(5, direction);
    TEST_ASSERT_EQUAL_FLOAT(960, character.x);
    TEST_ASSERT_EQUAL_FLOAT(540, character.y);
}

// Test cases for get_Rectdest_attacks function
void test_get_Rectdest_attacks(void) {
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
    
    SDL_Rect rect = get_Rectdest_attacks(0,character);
    TEST_ASSERT_EQUAL_INT((int)character.x - (int)character.width / 4, rect.x);
    TEST_ASSERT_EQUAL_INT((int)character.y + (int)character.height, rect.y);
    TEST_ASSERT_EQUAL_INT((int)character.width * 1.5, rect.w);
    TEST_ASSERT_EQUAL_INT((int)character.height, rect.h);
}

// Test cases for get_melee_direction function
void test_get_melee_direction(void) {
    int direction = get_melee_direction(0, 0, 10, 10);
    TEST_ASSERT_EQUAL_INT(7, direction);
    
    direction = get_melee_direction(0, 0, -10, -10);
    TEST_ASSERT_EQUAL_INT(3, direction);
}
