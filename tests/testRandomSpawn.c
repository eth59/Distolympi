#include "../Unity/src/unity.h"
#include "testRandomSpawn.h"
#include "../randomSpawn.h"
#include <stdlib.h>

void test_getMapHoles() {
    int index1 = 1;
    char *output1 = getMapHoles(index1);
    TEST_ASSERT_EQUAL_STRING("assets/mapholes01.txt", output1);
    free(output1);

    int index2 = 15;
    char *output2 = getMapHoles(index2);
    TEST_ASSERT_EQUAL_STRING("assets/mapholes15.txt", output2);
    free(output2);
}

void test_getCoordFromTiles() {
    int tileNumber = 2;
    int xTile;
    int yTile;
    getCoordFromTiles(tileNumber, &xTile, &yTile);
    TEST_ASSERT_EQUAL(xTile, 2);
    TEST_ASSERT_EQUAL(yTile, 0);
}