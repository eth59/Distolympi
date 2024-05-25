#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "structures.h"
#include "animations.h"
#include "inventory.h"


// Initialisation de l'inventaire
void init_inventory(Inventory *inventory) {
    inventory->count = 0;
}


// Ajout d'un objet à l'inventaire
void add_to_inventory(Inventory *inventory, Object *object) {
    // Vérifier si l'inventaire est plein
    if (inventory->count >= MAX_INVENTORY_SIZE) {
        printf("Inventaire plein !\n");
        return;
    }
    inventory->items[inventory->count] = *object;
    inventory->count++;
}


void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture) {
    SDL_Rect inventoryRect = {0, SCREEN_HEIGHT - SCREEN_HEIGHT/9, SCREEN_WIDTH, SCREEN_HEIGHT/9};
    // Dessiner un rectangle temporaire à la position et à la taille de l'inventaire
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &inventoryRect);
    SDL_RenderCopy(renderer, inventoryTexture, NULL, &inventoryRect);

    // Définir la position et la taille d'un slot d'inventaire
    SDL_Rect slotRect = {100, SCREEN_HEIGHT - SCREEN_HEIGHT/9 + 10, SCREEN_WIDTH/16 - 20, SCREEN_HEIGHT/9 - 20};

    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        if (i < character.inventory.count) {
            SDL_Texture* itemTexture = character.inventory.items[i].texture;
            SDL_Rect itemRect = {slotRect.x, slotRect.y, SCREEN_WIDTH/16 - 20, SCREEN_HEIGHT/9 - 20};
            SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);
        }
        // Déplacer le rectangle du slot vers la droite pour le prochain slot
        slotRect.x += 300;
    }
}


void draw_health_bar(SDL_Renderer* renderer, int x, int y, int current_health, int max_health) {
    // Dessiner le rectangle vide de la barre de vie
    int w = 500;
    SDL_Rect health_bar_rect = { x, y, w, SCREEN_HEIGHT/9 - 20 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &health_bar_rect);

    // Calculer la largeur du rectangle de santé actuel
    int health_width = (current_health * w) / max_health;
    if (health_width < 0) {
        health_width = 0;
    }

    // Dessiner le rectangle plein de la santé actuelle
    SDL_Rect health_rect = { x, y, health_width, SCREEN_HEIGHT/9 - 20 };
    if (current_health <= 25) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // rouge si la santé est inférieure ou égale à 25
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // vert sinon
    }
    SDL_RenderFillRect(renderer, &health_rect);
}
