#define MAX_INVENTORY_SIZE 9
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define FPS 240

// Structure pour caractériser les objets
typedef struct {
    float x;
    float y;
    int width;
    int height;
    int type; // 0 : nourriture, 1 : armes, 2 : équipements 
    int ground; // 0 : inventaire, 1 : sol
} Object;

// Structure pour caractériser l'inventaire
typedef struct {
    Object items[MAX_INVENTORY_SIZE];
    int count; // Nombre d'objets dans l'inventaire
} Inventory;

// Structure pour caractériser le personnage principal
typedef struct {
    float x;
    float y;
    int width;
    int height;
    float speed;
    float xHitBox;
    float yHitBox;
    float hitBoxWidth;
    float hitBoxHeight;
    float attack_speed;
    int attack_damage;
    int health;
    int max_health;
    Inventory inventory;
} Character;

// Structure pour caractériser une texture
typedef struct {
    SDL_Texture* texture;
    int type;
} Item;