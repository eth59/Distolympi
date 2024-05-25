#define MAX_RANDOM_OBJECTS 4
#define MAX_OBJECT_WIDTH 100
#define MAX_OBJECT_HEIGHT 100
#define MIN_OBJECT_WIDTH 50
#define MIN_OBJECT_HEIGHT 50
#define MAX_ITEM_TYPES 4

Object init_object(float x, float y, int width, int height, int type, int ground, SDL_Texture* texture);
void use_object(Character *character, Object *object, int selected_item);
int check_object_collision(Object* object, Character* character);
Object* generate_random_objects(int tailleMapHoles, int* tabMapHoles, SDL_Renderer* renderer, int numRandomObjects, Object* randomObjects);