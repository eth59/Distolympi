#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../structures.h"
#include "../inventory.h"
#include "../Unity/src/unity.h"


void test_init_inventory() {
    Inventory* inventory;
    init_inventory(inventory);
    TEST_ASSERT_EQUAL(inventory->count, 0);
}