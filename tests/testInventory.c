#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../structures.h"
#include "../inventory.h"
#include "../objects.h"
#include "../animations.h"
#include "../Unity/src/unity.h"


void test_init_inventory() {
    Inventory* inventory = malloc(sizeof(Inventory));
    init_inventory(inventory);
    TEST_ASSERT_EQUAL(inventory->count, 0);
    free(inventory);
}


void test_add_to_inventory() {
    Inventory* inventory = malloc(sizeof(Inventory));
    init_inventory(inventory);
    Object beret = init_object(100,100,100,100,2,1,NULL);
    int initial = inventory->count;
    add_to_inventory(inventory,&beret);
    TEST_ASSERT_EQUAL_INT(initial+1,inventory->count);
    free(inventory);
}


void test_draw_health_bar(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Effacer le renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Dessiner la barre de santé
    draw_health_bar(renderer, 50, 50, 50, 100); // 50% de santé

    // Vérifier les pixels du rendu
    SDL_Rect rect = {50, 50, 400, SCREEN_HEIGHT/9 - 20}; // Barre de santé de moitié pleine (800/2)
    SDL_Surface* surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    SDL_RenderReadPixels(renderer, &rect, surface->format->format, surface->pixels, surface->pitch);

    Uint32* pixels = (Uint32*)surface->pixels;
    Uint32 expected_color = SDL_MapRGBA(surface->format, 0, 255, 0, 255); // Vert

    for (int y = 0; y < rect.h; ++y) {
        for (int x = 0; x < rect.w; ++x) {
            TEST_ASSERT_EQUAL_UINT32(expected_color, pixels[y * (surface->pitch / 4) + x]);
        }
    }

    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void test_draw_inventory_bar(void) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* inventoryTexture;
    get_texture(&inventoryTexture, "./assets/inventory.png", renderer);
    if (!inventoryTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading inventory texture: %s", SDL_GetError());
        exit(-1);
    }
    TEST_ASSERT_NOT_NULL(inventoryTexture);

    Inventory* inventory = malloc(sizeof(Inventory));
    init_inventory(inventory);
    inventory->count = 1;
    Object beret = init_object(100,100,100,100,2,1,NULL);
    inventory->items[0] = beret;

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
        .inventory = *inventory
    };

    draw_inventory_bar(renderer, character, inventoryTexture);
    TEST_ASSERT_NOT_NULL(renderer);

    free(inventory);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}