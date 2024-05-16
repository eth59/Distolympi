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

// Fonction pour charger les textures des objets
void load_item_textures(SDL_Renderer* renderer, Item* items, char* item_path, int type) {
    // Charger la texture du fromage
    items[0].texture = get_texture(item_path, renderer);
    items[0].type = type;
}

void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture, Item* items) {
    // Définir la position et la taille de l'inventaire
    SDL_Rect inventoryRect = {0, SCREEN_HEIGHT - SCREEN_HEIGHT/9, SCREEN_WIDTH, SCREEN_HEIGHT/9};

    // Dessiner un rectangle temporaire à la position et à la taille de l'inventaire
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &inventoryRect);

    // Dessiner la barre d'inventaire
    SDL_RenderCopy(renderer, inventoryTexture, NULL, &inventoryRect);

    // Définir la position et la taille d'un slot d'inventaire
    SDL_Rect slotRect = {10, SCREEN_HEIGHT - SCREEN_HEIGHT/9 + 10, SCREEN_WIDTH/16 - 20, SCREEN_HEIGHT/9 - 20};

    // Parcourir tous les slots d'inventaire
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
        // Vérifier si le slot d'inventaire est occupé
        if (i < character.inventory.count) {
            // Récupérer la texture de l'item dans le tableau
            SDL_Texture* itemTexture = items[character.inventory.items[i].type].texture;

            // Définir la position et la taille de l'objet dans le slot
            SDL_Rect itemRect = {slotRect.x, slotRect.y, SCREEN_WIDTH/16 - 20, SCREEN_HEIGHT/9 - 20};

            // Dessiner l'item
            SDL_RenderCopy(renderer, itemTexture, NULL, &itemRect);
        }

        // Déplacer le rectangle du slot vers la droite pour le prochain slot
        slotRect.x += 200;
    }
}

void draw_health_bar(SDL_Renderer* renderer, int x, int y, int current_health, int max_health) {
    // Dessiner le rectangle vide de la barre de vie
    SDL_Rect health_bar_rect = { x, y, 800, SCREEN_HEIGHT/9 - 20 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &health_bar_rect);

    // Calculer la largeur du rectangle de santé actuel
    int health_width = (current_health * 800) / max_health;
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

// Fonction pour initialiser un objet
Object init_object(float x, float y, int width, int height, int type, int ground) {
    Object obj = {
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .type = type,
        .ground = ground
    };
    return obj;
}

void use_selected_object(Character *character, int *selected_item) {
    if (*selected_item >= 0 && *selected_item < MAX_INVENTORY_SIZE && 
    character->inventory.items[*selected_item].ground == 0) {
        Object *selected_object = &character->inventory.items[*selected_item];
        use_object(character, selected_object, *selected_item);
        *selected_item = -1; // Réinitialiser l'index de l'objet sélectionné
    }
}

void use_object(Character *character, Object *object, int selected_item) {
    if (object->type == 0) { // Si l'objet est de type nourriture
        int health_increase = 10; // Augmentation des points de vie
        character->health = fmin(character->health + health_increase, character->max_health); // sans dépasser la valeur maximale
    } else if (object->type == 1) { // Si l'objet est une arme
        character->attack_damage += 10; // Augmentation des dégâts d'attaque
    } else { // Si l'objet est un équipement
        character->speed += 5; // Augmentation de la vitesse du personnage
    }
    // Pour supprimer l'objet de l'inventaire
    for (int i = selected_item; i < character->inventory.count - 1; i++) {
        character->inventory.items[i] = character->inventory.items[i + 1];
    }
    character->inventory.count--;
}
