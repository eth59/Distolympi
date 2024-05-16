#define PI 3.14159265358979323846264338327950288 // Relou vs code et son erreur alors que M_PI existe mais bref
#define MAP_WIDTH 14 // Largeur de la map (hors mur extérieur)
#define MAP_HEIGHT 6 // Hauteur de la map (hors mur extérieur)
#define V (MAP_WIDTH*MAP_HEIGHT) // Taille de la map (hors mur extérieur) pour dijkstra

// Structure pour les ennemis
typedef struct {
    float x;
    float y;
    int width; // largeur pour le rendu
    int height; // hauteur pour le rendu
    int currentVertex;
    int nextVertex;
    float speed;
    int direction; // Pour l'animation
    int animation_frame; // Pour l'animation, l'étape du cycle d'animation
    int delay_frame; // Pour l'animation, ne pas actualiser à chaque rendu
    int max_column_frame; // Pour le sprite
    int max_line_frame; // Pour le sprite
    int attack_damage;
    int health;
    int isInsidePlayer; // 1 si le zombie peut attaquer le joueur, 0 sinon
    int range; // Portée d'attaque
    int isAShooter; // 1 si le zombie peut tirer, 0 sinon
} Enemy;

void init_zombie(Enemy **zombie, SDL_Renderer *renderer, SDL_Texture **zombieTexture, SDL_Rect **zombieRectSrc, int isAShooter);
void render_zombie(Enemy *zombie, SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRectSrc);
int zombie_is_in_range(Enemy *zombie, Character *player);
void move_zombie(Enemy **zombie, Character *player);
int **readMapCollisionFile(char *fileName);
float **mapToGraph(int **map);
int minDistance(float dist[], int vu[]);
void printSolution(float dist[]); // ====== TEMPORAIRE =======
int dijkstra(float **graph, int src, int dest);
void pathfinding(Enemy *zombie, Character *character, char *collisionTableFileName);
