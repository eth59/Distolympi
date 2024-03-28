#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "structures.h"
#include "animations.h"
#include "inventory.h"


// Fonction pour initialiser l'inventaire
void init_inventory(Inventory *inventory) {
    inventory->count = 0;
}

// Fonction pour ajouter un objet à l'inventaire
void add_to_inventory(Inventory *inventory, Object item) {
    if (inventory->count < MAX_INVENTORY_SIZE) {
        inventory->items[inventory->count++] = item;
    } else {
        printf("Inventory is full\n");
    }
}

// Fonction pour ajouter un objet à l'inventaire
void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture) {
    // Définir la position et la taille de l'inventaire
    SDL_Rect inventoryRect = {0, SCREEN_HEIGHT - SCREEN_HEIGHT/9, SCREEN_WIDTH, SCREEN_HEIGHT/9};

    // Dessiner un rectangle temporaire à la position et à la taille de l'inventaire
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &inventoryRect);

    // Dessiner la barre d'inventaire
    SDL_RenderCopy(renderer, inventoryTexture, NULL, &inventoryRect);

    // Définir la position et la taille d'un slot d'inventaire
    SDL_Rect slotRect = {10, SCREEN_HEIGHT - 190, 180, 180};

    // Parcourir tous les slots d'inventaire
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        // Vérifier si le slot d'inventaire est occupé
        if (i < character.inventory.count) {            
            // Définir la position et la taille de l'objet dans le slot
            SDL_Rect itemRect = {slotRect.x + 10, slotRect.y + 10, 160, 160};
            SDL_Texture* itemTexture = get_texture("assets/cheese.png", renderer);
            SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);
            SDL_DestroyTexture(itemTexture);
        }
        
        // Déplacer le rectangle du slot vers la droite pour le prochain slot
        slotRect.x += 200;
    }
}

