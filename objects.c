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

// Générer des coordonnées aléatoires dans la plage de la carte
void generate_random_coordinates(Object *object, int width, int height) {
    object->x = rand() % (SCREEN_WIDTH - width);
    object->y = rand() % (SCREEN_HEIGHT - height);
}

// Générer un nombre aléatoire entre 0 et le nombre maximal d'objets - 1
int generate_random_item_type() {
    return rand() % MAX_ITEM_TYPES;
}

void generate_random_objects(Object *objects, int num_objects, int width, int height) {
    for (int i = 0; i < num_objects; i++) {
        // Sélectionner aléatoirement le type d'objet
        int type = generate_random_item_type();
        
        // Initialiser l'objet avec des coordonnées aléatoires et le type sélectionné
        Object object = init_object(0, 0, width, height, type, 1);
        generate_random_coordinates(&object, width, height);
        
        // Copier l'objet généré dans le tableau d'objets
        objects[i] = object;
    }
}
