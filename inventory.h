void init_inventory(Inventory *inventory);
void add_to_inventory(Inventory *inventory, Object *object);
void remove_from_inventory(Inventory *inventory, int index);
void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture);
void draw_health_bar(SDL_Renderer* renderer, int x, int y, int current_health, int max_health);
