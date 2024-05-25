#include "../Unity/src/unity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../animations.h"
#include "testAnimations.h"

void test_get_texture_valid_image(void) {
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_HIDDEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = NULL;
    
    // Provide a valid path to an existing image
    char *valid_path = "assets/loli.png";

    
    get_texture(&texture, valid_path, renderer);
    TEST_ASSERT_NOT_NULL(texture);
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


void test_get_frames(void) {
    SDL_Rect *frames = NULL;
    int width_frame = 32;
    int height_frame = 32;
    int max_line_frame = 2;
    int max_column_frame = 2;

    get_frames(&frames, width_frame, height_frame, max_line_frame, max_column_frame);

    TEST_ASSERT_NOT_NULL(frames);

    for (int i = 0; i < max_line_frame * max_column_frame; i++) {
        int expected_x = (i % max_column_frame) * width_frame;
        int expected_y = (i / max_column_frame) * height_frame;
        TEST_ASSERT_EQUAL_INT(expected_x, frames[i].x);
        TEST_ASSERT_EQUAL_INT(expected_y, frames[i].y);
        TEST_ASSERT_EQUAL_INT(width_frame, frames[i].w);
        TEST_ASSERT_EQUAL_INT(height_frame, frames[i].h);
    }

    free(frames);
}
