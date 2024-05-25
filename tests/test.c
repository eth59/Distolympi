#include "testInventory.h"
#include "testObjects.h"
#include "../Unity/src/unity.h"

void setUp()
{

}

void tearDown()
{

}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_init_objects);
    // RUN_TEST(test_use_selected_objects_and_use_object);
    // RUN_TEST(test_check_object_collision);
    // RUN_TEST(test_generate_random_objects);

    UNITY_END();
    return 0;
}