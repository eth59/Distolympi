#include <SDL2/SDL.h>
#include "structures.h"
#include "objects.h"
#include "inventory.h"

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

void check_object_collision(Object* object, Character* character, int* objectAdded, SDL_Texture* objectTexture) {
    if (!*objectAdded && character->x + character->width >= object->x && character->x <= object->x + object->width &&
        character->y + character->height >= object->y && character->y <= object->y + object->height) {
        add_to_inventory(&character->inventory, *object);
        object->ground = 0;
        SDL_DestroyTexture(objectTexture);
        *objectAdded = 1;
    }
}
