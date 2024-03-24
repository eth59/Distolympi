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
} Character;

// Structure pour caractériser les objets
typedef struct {
    float x;
    float y;
    int width;
    int height;
    int type; // 0 : nourriture, 1 : armes, 2 : équipements 
    int ground; // 0 : inventaire, 1 : sol
} Object;
