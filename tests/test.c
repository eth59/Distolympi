#include "../Unity/src/unity.h"
#include "testEnemies.h"
#include "testInventory.h"
#include "testObjects.h"
#include "testAnimations.h"
#include "testCharacter.h"
#include "testSelectRandomMap.h"
#include "testCollisionHandling.h"
#include "testRandomSpawn.h"    
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

    // tests pour les objets
    printf("\n-----------------------\n");
    printf("Test Objects\n");
    RUN_TEST(test_init_objects);
    RUN_TEST(test_use_object);
    RUN_TEST(test_check_object_collision);
    RUN_TEST(test_generate_random_objects);

    // tests pour l'inventaire
    printf("\n-----------------------\n");
    printf("Test Inventory\n");
    RUN_TEST(test_init_inventory);
    RUN_TEST(test_add_to_inventory);
    RUN_TEST(test_draw_inventory_bar); 
    RUN_TEST(test_draw_health_bar);  

    // tests pour les animations
    printf("\n-----------------------\n");
    printf("Test Animations\n");
    RUN_TEST(test_get_texture_valid_image);
    RUN_TEST(test_get_frames);

    // test pour le character
    printf("\n-----------------------\n");
    printf("Test Character\n");
    RUN_TEST(test_moveCharacter);
    RUN_TEST(test_get_direction_and_move);
    RUN_TEST(test_get_Rectdest_attacks);
    RUN_TEST(test_get_melee_direction);

    // tests pour collisionHandling
    printf("\n-----------------------\n");
    printf("Test Collision Handling\n");
    RUN_TEST(test_openCollisionFile);
    RUN_TEST(test_checkCollision);

    // tests pour la séléction aléatoire de la map
    printf("\n-----------------------\n");
    printf("Test Selection Random Map\n");
    RUN_TEST(test_randomMapIndex);
    RUN_TEST(test_getMapFromIndex);
    RUN_TEST(test_getColliderTable);

    // tests pour les enemies
    printf("\n-----------------------\n");
    printf("Test Enemies\n");
    RUN_TEST(test_init_zombie);
    RUN_TEST(test_add_medal);
    RUN_TEST(test_init_medal);
    RUN_TEST(test_remove_medal);
    RUN_TEST(test_zombie_is_in_range);
    RUN_TEST(test_calculate_shooting_angle);
    RUN_TEST(test_zombie_attack);
    RUN_TEST(test_medal_touch_player);
    RUN_TEST(test_move_zombie);
    RUN_TEST(test_read_map_collision_file);
    RUN_TEST(test_map_to_graph);
    RUN_TEST(test_min_distance);
    RUN_TEST(test_dijkstra);
    RUN_TEST(test_get_sommet);
    RUN_TEST(test_pathfinding);

    // tests pour le spawn random
    printf("\n-----------------------\n");
    printf("Test spawn random\n");
    RUN_TEST(test_getMapHoles);
    RUN_TEST(test_getCoordFromTiles);
    UNITY_END();
    return 0;
}
