#define MAX_ITEM_TYPES 4
#define MAX_RANDOM_OBJECTS 4
#define MAX_OBJECT_WIDTH 100
#define MAX_OBJECT_HEIGHT 100
#define MIN_OBJECT_WIDTH 50
#define MIN_OBJECT_HEIGHT 50

Object init_object(float x, float y, int width, int height, int type, int ground, SDL_Texture* texture);
void use_selected_object(Character *character, int *selected_item);
void use_object(Character *character, Object *object, int selected_item);
int check_object_collision(Object* object, Character* character);
void generate_random_coordinates(Object *object, int width, int height);
int generate_random_item_type();
void generate_random_objects(Object *objects, int num_objects, int width, int height); ;