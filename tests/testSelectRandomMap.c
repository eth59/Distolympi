#include "../Unity/src/unity.h"
#include "testSelectRandomMap.h"
#include "../selectRandomMap.h"
#include <stdlib.h>

void test_randomMapIndex() {
    int test1 = randomMapIndex();
    int bool1 = (0 < test1 && test1 <= 2);
    TEST_ASSERT_EQUAL(1, bool1);

    int test2 = randomMapIndex();
    int bool2 = (0 < test2 &&  test2 <= 2);
    TEST_ASSERT_EQUAL(1, bool2);
}

void test_getMapFromIndex() {
    int index1 = 1;
    char *output1 = getMapFromIndex(index1);
    TEST_ASSERT_EQUAL_STRING("assets/map01.png", output1);
    free(output1);

    int index2 = 15;
    char *output2 = getMapFromIndex(index2);
    TEST_ASSERT_EQUAL_STRING("assets/map15.png", output2);
    free(output2);
}

void test_getColliderTable() {
    int index1 = 8;
    char *output1 = getColliderTable(index1);
    TEST_ASSERT_EQUAL_STRING("assets/map08.txt", output1);
    free(output1);

    int index2 = 23;
    char *output2 = getColliderTable(index2);
    TEST_ASSERT_EQUAL_STRING("assets/map23.txt", output2);
    free(output2);
}