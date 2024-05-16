void init_inventory(Inventory *inventory);
void add_to_inventory(Inventory *inventory, Object item);
void remove_from_inventory(Inventory *inventory, int index);
void load_item_textures(SDL_Renderer* renderer, Item* items, char* item_path, int type);
void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture, Item* items);
void draw_health_bar(SDL_Renderer* renderer, int x, int y, int current_health, int max_health);
Object init_object(float x, float y, int width, int height, int type, int ground);
void use_selected_object(Character *character, int *selected_item);
void use_object(Character *character, Object *object, int selected_item);
