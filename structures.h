// Structure pour caractériser le personnage principal
typedef struct {
    float x;
    float y;
    int width;
    int height;
    float speed;
} Character;

// Structure pour caractériser les murs
// Permet en gros au personnage de pas traverser les murs si des murs sont affichés
// On y met la taille des murs en valeur absolue (pas de négatif pour la droite et en bas)
typedef struct {
    float left;
    float up;
    float right;
    float down;
} Wall;

// Structure pour caractériser les objets
typedef struct {
    float x;
    float y;
    int width;
    int height;
    int type; // 0 : nourriture, 1 : armes, 2 : équipements 
    int ground; // 0 : inventaire, 1 : sol
} Object;
