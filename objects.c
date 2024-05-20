#include <SDL2/SDL.h>
#include "structures.h"
#include "objects.h"
#include "inventory.h"


// Fonction pour initialiser un objet
Object init_object(float x, float y, int width, int height, int type, int ground, SDL_Texture* texture) {
    Object object;
    object.x = x;
    object.y = y;
    object.width = width;
    object.height = height;
    object.type = type;
    object.ground = ground;
    object.texture = NULL;
    return object;
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
    if (selected_item < 0 || selected_item >= character->inventory.count) {
        return;
    }
    if (object->type == 0) { // Si l'objet est de type nourriture, augmentation des points de vie
        int health_increase = 10;
        character->health = fmin(character->health + health_increase, character->max_health); // sans dépasser la valeur maximale
    } else if (object->type == 1) { // Si l'objet est une arme, augmentation des dégâts d'attaque
        character->attack_damage += 10;
    } else if (object->type == 2) { // Si l'objet est un équipement défensif, augmentation de sa capacité de vie
        character->max_health += 10; 
    } else { // Si l'objet est un équipement vitesse, augmentation de la vitesse du personnage
        character->speed += 5; 
    }
    // Pour supprimer l'objet de l'inventaire
    for (int i = selected_item; i < character->inventory.count - 1; i++) {
        character->inventory.items[i] = character->inventory.items[i + 1];
    }
    character->inventory.count--;
}

int check_object_collision(Object* object, Character* character) {
    SDL_Rect objectRect = { (int)object->x, (int)object->y, object->width, object->height };
    SDL_Rect characterRect = { (int)character->x, (int)character->y, character->width, character->height };

    if (SDL_HasIntersection(&objectRect, &characterRect) && object->ground == 1) {
        if (character->inventory.count < MAX_INVENTORY_SIZE) {
            character->inventory.items[character->inventory.count] = *object;
            character->inventory.items[character->inventory.count].ground = 0; // Déplacer l'objet à l'inventaire
            character->inventory.count++;
            object->ground = 0;
            return 1;
        }
    }
    return 0;
}
