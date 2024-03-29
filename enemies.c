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
    (*zombie)->nextX = 100;
    (*zombie)->nextY = 100;
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

void move_zombie(Enemy **zombie)
{
    printf("On va en : %d %d\n", (*zombie)->nextX, (*zombie)->nextY);
    int dx = (*zombie)->nextX - (*zombie)->x;
    int dy = (*zombie)->nextY - (*zombie)->y;

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
    int **map = (int**)malloc((MAP_HEIGHT+2) * sizeof(int*));
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        int *temp = (int*)malloc((MAP_WIDTH+2) * sizeof(int));
        map[i] = temp;
    }

    // on ajoute tous les caractères du fichier a la chaine
    int i = 0, j = 0, character;
    while ((character = fgetc(file)) != EOF) {
        map[i][j] = character - '0';
        i += j == MAP_WIDTH+1;
        j = (j+1) % (MAP_WIDTH+2);
    } 
    fclose(file);    
    return map;
}

// On convertit la map qu'on a lu du fichier en un graphe pour dijkstra
float **mapToGraph(int **map)
{
    // Mémoire mémoire je veux toujours plus de mémoire
    float **graph = (float**)malloc(V * sizeof(float*));
    for (int v = 0; v < V; v++)
    {
        float *temp = (float*)calloc(V, sizeof(float));
        graph[v] = temp;
    }

    // On met les arrêtes dans le graphe avec un poids de 1 pour les cases à côté
    // Donc un poids de sqrt(2) en diagonale (merci pythagore)
    for (int i = 1; i < MAP_HEIGHT+1; i++)
    {
        for (int j = 1; j < MAP_WIDTH+1; j++)
        {
            if (map[i][j] == 0)
            {            
                if (map[i][j+1] == 0)
                {
                    // On regarde à droite
                    graph[(i-1)*MAP_WIDTH+j-1][(i-1)*MAP_WIDTH+j] = 1;
                    graph[(i-1)*MAP_WIDTH+j][(i-1)*MAP_WIDTH+j-1] = 1;
                }
                if (map[i+1][j+1] == 0)
                {
                    // On regarde en bas à droite
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j] = sqrt(2);
                    graph[i*MAP_WIDTH+j][(i-1)*MAP_WIDTH+j-1] = sqrt(2);
                }
                if (map[i+1][j] == 0)
                {
                    // On regarde en bas
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j-1] = 1;
                    graph[i*MAP_WIDTH+j-1][(i-1)*MAP_WIDTH+j-1] = 1;
                }
                if (map[i+1][j-1] == 0)
                {
                    // On regarde en bas à gauche
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j-2] = sqrt(2);
                    graph[i*MAP_WIDTH+j-2][(i-1)*MAP_WIDTH+j-1] = sqrt(2);
                }   
            }
        }
    }
    return graph;
}

// Calcule le sommet à distance minimale pour dijkstra
int minDistance(float dist[], int vu[])
{
    float min = FLT_MAX;
    int min_index;

    for (int v = 0; v < V; v++)
        if (!vu[v] && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

// Dijkstra
int dijkstra(float **graph, int src, int dest)
{
    float dist[V]; // Le tableau où on va stocker les distances à la src
    for (int i = 0; i < V; i++)
    {
        dist[i] = FLT_MAX;
    }
    dist[src] = 0; // La seule valeur qu'on connaît
    int vu[V] = {0}; // Pour savoir les sommets qu'on a déjà vu
    int pred[V]; // Tableau avec les prédécesseurs dans le parcours

    // boucle principale de dijkstra
    for (int i = 0; i < V-1; i++)
    {
        // On prend le sommet qu'on a pas vu le + proche
        int u = minDistance(dist, vu);
        vu[u] = 1; // du coup on l'a vu

        // On change les distances des voisins
        for (int v = 0; v < V; v++)
        {
            if (graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v]; // On met à jour la distance
                pred[v] = u; // On stocke le prédécesseur
            }
        }
    }
    int u = dest;
    while (pred[u] != src)
    {
        u = pred[u];
    }
    return u; // on retourne la prochaine tile sur laquelle le zombie doit aller
} 

// Pathfinding
void pathfinding(Enemy *zombie, Character *character, char *collisionTableFileName)
{
    printf("%s\n", collisionTableFileName);
    int** map = readMapCollisionFile(collisionTableFileName);
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        for (int j = 0; j < MAP_WIDTH+2; j++)
        {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    float** graph = mapToGraph(map);
    int tileWidth = 1920 / MAP_WIDTH;
    int tileHeight = 1080 / MAP_HEIGHT;
    int zombieTileX = (int)(zombie->x / tileWidth);
    int zombieTileY = (int)(zombie->y / tileHeight);
    int zombieSommet = zombieTileY * MAP_WIDTH + zombieTileX;
    printf("On est en %d.\n"), zombieSommet;
    int characterTileX = (int)(character->x / tileWidth);
    int characterTileY = (int)(character->y / tileHeight);
    int characterSommet = characterTileY * MAP_WIDTH + characterTileX;
    int nextSommet = dijkstra(graph, zombieSommet, characterSommet);
    printf("On doit aller en %d.\n", nextSommet);
    zombie->nextX = ((nextSommet / MAP_WIDTH)+1)*tileHeight; // il y a un +1 pour le mur du haut
    zombie->nextY = ((nextSommet % MAP_WIDTH)+1)*tileWidth; // il y a un +1 pour le mur de gauche

    // FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE FREE
    for (int i = 0; i < MAP_HEIGHT+2; i++)
    {
        free(map[i]);
    }
    free(map);
    for (int i = 0; i < V; i++)
    {
        free(graph[i]);
    }
    free(graph);
}