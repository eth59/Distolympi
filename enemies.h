// Structure pour les ennemis
typedef struct {
    float x;
    float y;
    int width; // largeur pour le rendu
    int height; // hauteur pour le rendu
    float speed;
    int direction; // Pour l'animation
    int animation_frame; // Pour l'animation, l'étape du cycle d'animation
    int delay_frame; // Pour l'animation, ne pas actualiser à chaque rendu
    int max_column_frame; // Pour le sprite
    int max_line_frame; // Pour le sprite
    int attack_damage;
    int health;
} Enemy;

void init_zombie(Enemy **zombie, SDL_Renderer *renderer, SDL_Texture **zombieTexture, SDL_Rect **zombieRectSrc);
void render_zombie(Enemy *zombie, SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRectSrc);
void move_zombie(Enemy **zombie, Character *character);