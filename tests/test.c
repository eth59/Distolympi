#include "../Unity/src/unity.h"
#include "testInventory.h"
#include "testObjects.h"
#include "testAnimations.h"
#include "testCharacter.h"
#include "testSelectRandomMap.h"
#include "testCollisionHandling.h"
#include "../Unity/src/unity.h"

void setUp()
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
}

void tearDown()
{ 
    IMG_Quit();
    SDL_Quit();
}

int main(void) {
    UNITY_BEGIN();

    /*RUN_TEST(test_init_objects);
    //RUN_TEST(test_use_selected_objects_and_use_object);
    RUN_TEST(test_check_object_collision);
    RUN_TEST(test_generate_random_objects);

    */
    RUN_TEST(test_get_texture_valid_image);
    RUN_TEST(test_get_frames);

    //
    RUN_TEST(test_get_direction_and_move);
    RUN_TEST(test_get_Rectdest_attacks);
    RUN_TEST(test_get_melee_direction);

    // tests pour collisionHandling
    RUN_TEST(test_openCollisionFile);
    RUN_TEST(test_checkCollision);

    // tests pour la séléction aléatoire de la map
    RUN_TEST(test_randomMapIndex);
    RUN_TEST(test_getMapFromIndex);
    RUN_TEST(test_getColliderTable);

    UNITY_END();
    return 0;
}
