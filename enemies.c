#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "limits.h"
#include "animations.h"
#include "structures.h"
#include "enemies.h"

void init_zombie(Enemy **zombie, SDL_Renderer *renderer, SDL_Texture **zombieTexture, SDL_Rect **zombieRectSrc)
{
    // Chargement de la texture
    *zombieTexture = get_texture("assets/zombie.png", renderer);

    // Création de l'objet
    *zombie = malloc(sizeof(Enemy));
    (*zombie)->x = 100;
    (*zombie)->y = 100;
    (*zombie)->width = 130;
    (*zombie)->height = 130;
    (*zombie)->speed = 1.5;
    (*zombie)->direction = 0;
    (*zombie)->animation_frame = 0;
    (*zombie)->delay_frame = 0;
    (*zombie)->max_column_frame = 8;
    (*zombie)->max_line_frame = 10;
    (*zombie)->attack_damage = 5;
    (*zombie)->health = 30;
    
    // On divise le tileset du zombie
    *zombieRectSrc = get_frames(32, 32, (*zombie)->max_line_frame, (*zombie)->max_column_frame);
}

void render_zombie(Enemy *zombie, SDL_Renderer *renderer, SDL_Texture *zombieTexture, SDL_Rect *zombieRectSrc)
{
    SDL_Rect zombieRectDest = {(int)zombie->x, (int)zombie->y, zombie->width, zombie->height};
    int sprite_nb = zombie->direction * zombie->max_column_frame + zombie->animation_frame;
    SDL_RenderCopy(renderer, zombieTexture, &zombieRectSrc[sprite_nb], &zombieRectDest);
}

void move_zombie(Enemy **zombie, Character *character)
{
    float dx = character->x - (*zombie)->x;
    float dy = character->y - (*zombie)->y;

    // Calcul de l'angle entre le zombie et le joueur
    // On redéfinit PI parce que flemme de l'erreur de vs code
    float angle = atan2(dy, dx) * 180 / PI; 

    // Convertion de l'angle en une direction + déplacement du zombie
    // On multiplie par sqrt(2) en diagonale
    // pour éviter une impression de vitesse plus élevée
    if (angle >= -22.5 && angle < 22.5) {
        // Droite
        (*zombie)->direction = 8;
        (*zombie)->x += (*zombie)->speed;
    } else if (angle >= 22.5 && angle < 67.5) {
        // Bas droite
        (*zombie)->direction = 9;
        (*zombie)->x += (*zombie)->speed*0.7071f;
        (*zombie)->y += (*zombie)->speed*0.7071f;
    } else if (angle >= 67.5 && angle < 112.5) {
        // Bas
        (*zombie)->direction = 2;
        (*zombie)->y += (*zombie)->speed;
    } else if (angle >= 112.5 && angle < 157.5) {
        // Bas gauche
        (*zombie)->direction = 3;
        (*zombie)->x -= (*zombie)->speed*0.7071f;
        (*zombie)->y += (*zombie)->speed*0.7071f;
    } else if (angle >= 157.5 || angle < -157.5) {
        // Gauche
        (*zombie)->direction = 4;
        (*zombie)->x -= (*zombie)->speed;
    } else if (angle >= -157.5 && angle < -112.5) {
        // Haut gauche
        (*zombie)->direction = 5;
        (*zombie)->x -= (*zombie)->speed*0.7071f;
        (*zombie)->y -= (*zombie)->speed*0.7071f;
    } else if (angle >= -112.5 && angle < -67.5) {
        // Haut
        (*zombie)->direction = 6;
        (*zombie)->y -= (*zombie)->speed;
    } else {
        // Haut droite
        (*zombie)->direction = 7;
        (*zombie)->x += (*zombie)->speed*0.7071f;
        (*zombie)->y -= (*zombie)->speed*0.7071f;
    }

    // On met à jour les paramètres de l'animation
    if ((*zombie)->delay_frame > 20 / (*zombie)->speed)
    {
        (*zombie)->animation_frame = ((*zombie)->animation_frame + 1) % (*zombie)->max_column_frame;
        (*zombie)->delay_frame = 0;
    }
    (*zombie)->delay_frame++;
}

// Pour lire le fichier de collision et en faire une matrice
int **readMapCollisionFile(char *fileName)
{
    
    FILE *file;
    // Ouvrir le fichier 
    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("erreur");
        return NULL;
    }
    
    // Mémoire mémoire je veux de la mémoire
    int **map = (int**)malloc(MAP_HEIGHT * sizeof(int*));
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        int *temp = (int*)malloc(MAP_WIDTH * sizeof(int));
        map[i] = temp;
    }

    // on ajoute tous les caractères du fichier a la chaine
    int i = 0, j = 0, character;
    while ((character = fgetc(file)) != EOF) { 
        map[i][j] = (int)character - (int)'0';
        j += i % MAP_WIDTH;
        i = (i+1) / MAP_WIDTH;
    } 
    fclose(file);
    
    return map;
}

// On convertit la map qu'on a lu du fichier en un graphe pour dijkstra
int **mapToGraph(int **map)
{
    // Mémoire mémoire je veux toujours plus de mémoire
    int **graph = (int**)malloc(V * sizeof(int*));
    for (int v = 0; v < V; v++)
    {
        int *temp = (int*)calloc(V, sizeof(int));
        graph[v] = temp;
    }

    // On parcourt la map, si c'est pas 0
    // On regarde les 8 directions alentours et si c pas 0, on a trouvé une arrête

    return graph;
}

// Calcule le sommet à distance minimale pour dijkstra
int minDistance(int dist[], int vu[])
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++)
        if (!vu[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    
    return min_index;
}

// ========== TEMPORAIRE =======
void printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t\t\t %d\n", i, dist[i]);
}

// Dijkstra
void dijkstra(int **graph[V][V], int src)
{
    int dist[V]; // Le tableau de retour avec toutes les distances
    int vu[V]; // Pour garder en mémoire les sommets vus

    // On cherche le chemin le plus court pour tous les sommets
    for (int i = 0; i < V-1; i++)
    {
        // On prend le sommet non traîté le + proche
        int u = minDistance(dist, vu);
        vu[u] = 1; // On le marque comme vu

        // On met à jour la distance des sommets adjacents
        for (int v = 0; v < V; v++)
            // On met à jour dist[v] ssi v n'a pas été vu
            // et il y a une arête entre u et v et que le chemin est plus court
            // en passant par u que par le chemin actuel
            // jusque v (ie de longueur dist[v])
            if (!vu[v] && graph[u][v] && dist[u] != INT_MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    printSolution(dist); // TEMPORAIRE  

    printf("===================\n\n\n");
} 

// Pathfinding
void pathfinding(Enemy *zombie, Character *character)
{
    int** map = createGraph("assets/map01.txt");
    // FAUT LE TRANSFORMER EN GRAPH
    int tileWidth = 1920 / MAP_WIDTH;
    int tileHeight = 1080 / MAP_HEIGHT;
    int zombieTileX = (int)(zombie->x / tileWidth);
    int zombieTileY = (int)(zombie->y / tileHeight);
    int zombieSommet = zombieTileY * MAP_WIDTH + zombieTileX;
    int characterTileX = (int)(character->x / tileWidth);
    int characterTileY = (int)(character->y / tileHeight);
    int characterSommet = characterTileY * MAP_WIDTH + characterTileX;
    dijkstra(graph, zombieSommet);
    printf("%d\n", characterSommet);

    // FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
}