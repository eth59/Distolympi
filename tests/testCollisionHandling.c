#include "../Unity/src/unity.h"
#include "testCollisionHandling.h"
#include "../collisionHandling.h"
#include <stdlib.h>

void test_openCollisionFile() {
    char *filename1 = "assets/map01.txt";
    char *output1 = openCollisionFile(filename1);
    TEST_ASSERT_NOT_NULL(output1);
    TEST_ASSERT_EQUAL_STRING("11111111111111111000010000000001101000000100010120000000000000022000000000000002100000000000001111000000100000011111111111111111", output1);
    free(output1);

    char *filename2 = "assets/map02.txt";
    char *output2 = openCollisionFile(filename2);
    TEST_ASSERT_NOT_NULL(output2);
    TEST_ASSERT_EQUAL_STRING("11111111111111111000000000010011110100000100000120000000000000022000000000000002100000000010000110011000000000011111111111111111", output2);
    free(output2);
}

void test_checkCollision() {
    char *collisionTable = "11111112111111111000010000000001101000000100010120000000000000022000000000000002100000000000001111000000100000011111111111112111";
    float x1 = 0; 
    float y1 = 0;
    TEST_ASSERT_EQUAL(1, checkCollision(collisionTable, x1, y1));

    float x2 = 125;
    float y2 = 125;
    TEST_ASSERT_EQUAL(0, checkCollision(collisionTable, x2, y2));
}
