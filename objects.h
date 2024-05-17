Object init_object(float x, float y, int width, int height, int type, int ground);
void use_selected_object(Character *character, int *selected_item);
void use_object(Character *character, Object *object, int selected_item);
void check_object_collision(Object* object, Character* character, int* objectAdded, SDL_Texture* objectTexture);
