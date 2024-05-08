void init_inventory(Inventory *inventory);
void add_to_inventory(Inventory *inventory, Object item);
void remove_from_inventory(Inventory *inventory, int index);
void load_item_textures(SDL_Renderer* renderer, Item* items);
void draw_inventory_bar(SDL_Renderer* renderer, Character character, SDL_Texture* inventoryTexture, Item* items);
void draw_health_bar(SDL_Renderer* renderer, int x, int y, int current_health, int max_health);
