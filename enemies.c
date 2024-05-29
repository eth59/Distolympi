#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "limits.h"
#include "animations.h"
#include "structures.h"
#include "enemies.h"
#include "randomSpawn.h"

void init_zombie(Enemy **zombie, SDL_Renderer *renderer, int isAShooter, int tailleMapHoles, int *tabMapHoles, int zombieIndex, int zombieNumber)
{
    // Chargement de la texture
    if (isAShooter)
    {
        get_texture(&(*zombie)->texture, "assets/zombie2.png", renderer);
    } else {
        get_texture(&(*zombie)->texture, "assets/zombie.png", renderer);
    }

    int indexHole = randomInt(tailleMapHoles/zombieNumber);
    indexHole = indexHole + zombieIndex*tailleMapHoles/zombieNumber;
    int tileNumber = tabMapHoles[indexHole];
    int xCoord;
    int yCoord;     
    getCoordFromTiles(tileNumber, &xCoord, &yCoord);    
    xCoord = xCoord*(SCREEN_WIDTH/16);
    yCoord = yCoord*(SCREEN_HEIGHT/9);
    (*zombie)->x = xCoord;
    (*zombie)->y = yCoord;
    (*zombie)->width = SCREEN_WIDTH / 16;
    (*zombie)->height = SCREEN_HEIGHT / 9;
    (*zombie)->currentVertex = 0;
    (*zombie)->nextVertex = 0;
    (*zombie)->speed = 1;
    (*zombie)->direction = 0;
    (*zombie)->animation_frame = 0;
    (*zombie)->delay_frame = 0;
    (*zombie)->max_column_frame = 8;
    (*zombie)->max_line_frame = 10;
    (*zombie)->attack_damage = (isAShooter) ? 10 : 5;
    (*zombie)->health = (isAShooter) ? 30 : 50;
    (*zombie)->isInsidePlayer = 0;
    (*zombie)->range = (isAShooter) ? 300 : 50;
    (*zombie)->isAShooter = isAShooter;
    (*zombie)->medals = NULL;
    (*zombie)->last_zombie_hit=2000;
    (*zombie)->attack_speed = (isAShooter) ? 0.5 : 1;
    
    // On divise le tileset du zombie
    get_frames(&(*zombie)->rectSrc, 32, 32, (*zombie)->max_line_frame, (*zombie)->max_column_frame);
}

void addMedal(Enemy* zombie, Medal medal) {
    MedalList* newMedalList = (MedalList*) malloc(sizeof(MedalList));
    newMedalList->medal = medal;
    newMedalList->next = zombie->medals;
    zombie->medals = newMedalList;
}

void init_medal(Enemy *zombie, Character *player, SDL_Renderer *renderer) 
{
    // Création de la médaille
    Medal medal;
    medal.x = zombie->x + zombie->width/2; // La médaille apparaît au milieu du zombie
    medal.y = zombie->y + zombie->height/2; // La médaille apparaît au milieu du zombie
    medal.width = 32;
    medal.height = 32;
    medal.speed = 2;
    medal.angle = calculateShootingAngle(zombie, player);
    int randomNumber = rand() % 3; // Normalement la seed est déjà initialisée quand on choisit la map mais si on l'enlève faut penser à le remettre quelque part
    switch (randomNumber)
    {
    case 0:
        get_texture(&medal.texture, "assets/gold_medal.png", renderer);
        break;
    case 1:
        get_texture(&medal.texture, "assets/silver_medal.png", renderer);
        break;
    default:
        get_texture(&medal.texture, "assets/bronze_medal.png", renderer);
        break;
    }

    // Ajout de la médaille à la liste des médailles
    addMedal(zombie, medal);
}

void removeMedal(Enemy* zombie, Medal medal) {
    MedalList* current = zombie->medals;
    MedalList* previous = NULL;

    while (current != NULL) {
        if (current->medal.x == medal.x && current->medal.y == medal.y) {
            if (previous == NULL) {
                zombie->medals = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void render_zombie(Enemy *zombie, SDL_Renderer *renderer)
{
    // Render du zombie
    SDL_Rect zombieRectDest = {(int)zombie->x, (int)zombie->y, zombie->width, zombie->height};
    int sprite_nb = zombie->direction * zombie->max_column_frame + zombie->animation_frame;
    SDL_RenderCopy(renderer, zombie->texture, &zombie->rectSrc[sprite_nb], &zombieRectDest);

    // Render des médailles
    MedalList* current = zombie->medals;
    while (current != NULL) {
        SDL_Rect medalRect = {(int)current->medal.x, (int)current->medal.y, current->medal.width, current->medal.height};
        SDL_RenderCopy(renderer, current->medal.texture, NULL, &medalRect);
        current = current->next;
    }
}

int zombie_is_in_range(Enemy *zombie, Character *player)
{
    return sqrt(pow(zombie->x - player->x, 2) + pow(zombie->y - player->y, 2)) <= zombie->range;
}

double calculateShootingAngle(Enemy *zombie, Character *player)
{
    // Il faut décaler la position du joueur et du zombie pour être centré
    double dx = player->x + player->width/2 - zombie->x - zombie->width/2;
    double dy = player->y + player->height/2 - zombie->y - zombie->height/2;
    return atan2(dy, dx);
}

void zombie_attack(Enemy *zombie, Character *player, SDL_Renderer *renderer)
{
    if (!zombie->isAShooter)
    {
        player->health = player->health - zombie->attack_damage;
    } else {
        init_medal(zombie, player, renderer);
    }
}

int medalTouchPlayer(Medal medal, Character *player) {
    // Vérifie si la médaille est à l'intérieur de la hitbox du joueur
    if (medal.x >= player->x && medal.x <= player->x + player->width &&
        medal.y >= player->y && medal.y <= player->y + player->height) {
        return 1;
    }
    return 0;
}

void move_zombie(Enemy **zombie, Character *player)
{
    // On déplace les médailles
    MedalList *current = (*zombie)->medals;
    MedalList *tmp; // Utiliser quand on supprime une médaille
    while (current != NULL)
    {
        double dx = current->medal.speed * cos(current->medal.angle);
        double dy = current->medal.speed * sin(current->medal.angle);
        current->medal.x += dx;
        current->medal.y += dy;

        if (current->medal.x < 50 || current->medal.x > 1870 || current->medal.y < 50 || current->medal.y > 1030 - 1920 / (MAP_WIDTH+2))
        {
            // Il faut détruire les médailles si elles sortent de l'écran
            tmp = current;
            current = current->next;
            removeMedal(*zombie, tmp->medal);
        } else if (medalTouchPlayer(current->medal, player))
        {
            // Il faut infliger des dégâts si on touche le joueur
            player->health = player->health - (*zombie)->attack_damage;
            tmp = current;
            current = current->next;
            removeMedal(*zombie, tmp->medal);
        } else {
            // J'avais oublié cette ligne, c'est une bonne idée si vous voulez tout faire freeze et faire un barbecue sur le cpu      
            current = current->next;
        }  
    }

    // On déplace le zombie
    if (!zombie_is_in_range(*zombie, player)){
        int diff = (*zombie)->nextVertex - (*zombie)->currentVertex;
        // On multiplie par sqrt(2) en diagonale
        // pour éviter une impression de vitesse plus élevée
        if (diff == 0) {
            // Sur la même case que le joueur
            int diff_x = player->x - (*zombie)->x;
            int diff_y = player->y - (*zombie)->y;
            double angle = atan2(diff_y, diff_x);
            int index = (int)round(angle / (PI / 4));
            if (index < 0) {
                index += 8;
            }
            switch (index)
            {
            case 6:
                // Haut
                (*zombie)->direction = 6;
                (*zombie)->y -= (*zombie)->speed;
                break;
            case 7:
                // Haut droite
                (*zombie)->direction = 7;
                (*zombie)->x += (*zombie)->speed*0.7071f;
                (*zombie)->y -= (*zombie)->speed*0.7071f;
                break;
            case 0:
                // Droite
                (*zombie)->direction = 8;
                (*zombie)->x += (*zombie)->speed;
                break;
            case 1:
                // Bas droite
                (*zombie)->direction = 9;
                (*zombie)->x += (*zombie)->speed*0.7071f;
                (*zombie)->y += (*zombie)->speed*0.7071f;
                break;
            case 2:
                // Bas
                (*zombie)->direction = 2;
                (*zombie)->y += (*zombie)->speed;
                break;
            case 3:
                // Bas gauche
                (*zombie)->direction = 3;
                (*zombie)->x -= (*zombie)->speed*0.7071f;
                (*zombie)->y += (*zombie)->speed*0.7071f;
                break;
            case 4:
                // Gauche
                (*zombie)->direction = 4;
                (*zombie)->x -= (*zombie)->speed;
                break;
            case 5:
                // Haut gauche
                (*zombie)->direction = 5;
                (*zombie)->x -= (*zombie)->speed*0.7071f;
                (*zombie)->y -= (*zombie)->speed*0.7071f;
                break;
            }
        }  else if (diff == 1) {
            // Droite
            (*zombie)->direction = 8;
            (*zombie)->x += (*zombie)->speed;
        } else if (diff == MAP_WIDTH + 1) {
            // Bas droite
            (*zombie)->direction = 9;
            (*zombie)->x += (*zombie)->speed*0.7071f;
            (*zombie)->y += (*zombie)->speed*0.7071f;
        } else if (diff == MAP_WIDTH) {
            // Bas
            (*zombie)->direction = 2;
            (*zombie)->y += (*zombie)->speed;
        } else if (diff == MAP_WIDTH - 1) {
            // Bas gauche
            (*zombie)->direction = 3;
            (*zombie)->x -= (*zombie)->speed*0.7071f;
            (*zombie)->y += (*zombie)->speed*0.7071f;
        } else if (diff == -1) {
            // Gauche
            (*zombie)->direction = 4;
            (*zombie)->x -= (*zombie)->speed;
        } else if (diff == -MAP_WIDTH - 1) {
            // Haut gauche
            (*zombie)->direction = 5;
            (*zombie)->x -= (*zombie)->speed*0.7071f;
            (*zombie)->y -= (*zombie)->speed*0.7071f;
        } else if (diff == -MAP_WIDTH) {
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
    int droite = 0;
    int gauche = 0;
    int bas = 0;
    for (int i = 1; i < MAP_HEIGHT+1; i++)
    {
        for (int j = 1; j < MAP_WIDTH+1; j++)
        {
            if (map[i][j] == 0)
            {            
                if (map[i][j+1] == 0)
                {
                    // On regarde à droite
                    droite = 1;
                    graph[(i-1)*MAP_WIDTH+j-1][(i-1)*MAP_WIDTH+j] = 1;
                    graph[(i-1)*MAP_WIDTH+j][(i-1)*MAP_WIDTH+j-1] = 1;
                }
                if (map[i][j-1] == 0)
                {
                    // On regarde à gauche
                    gauche = 1;
                }
                if (map[i+1][j] == 0)
                {
                    // On regarde en bas
                    bas = 1;
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j-1] = 1;
                    graph[i*MAP_WIDTH+j-1][(i-1)*MAP_WIDTH+j-1] = 1;
                }

                // Pour les diagonales, on veut y aller que si les cases à côté sont libres
                // Sinon l'ennemi marche sur un rocher et ça c'est pas cool
                if (map[i+1][j+1] == 0 && bas && droite)
                {
                    // On regarde en bas à droite
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j] = sqrt(2);
                    graph[i*MAP_WIDTH+j][(i-1)*MAP_WIDTH+j-1] = sqrt(2);
                }
                if (map[i+1][j-1] == 0 && bas && gauche)
                {
                    // On regarde en bas à gauche
                    graph[(i-1)*MAP_WIDTH+j-1][i*MAP_WIDTH+j-2] = sqrt(2);
                    graph[i*MAP_WIDTH+j-2][(i-1)*MAP_WIDTH+j-1] = sqrt(2);
                }   
            }
            droite = 0;
            gauche = 0;
            bas = 0;
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
    {
        if (!vu[v] && dist[v] <= min)
        {
            min = dist[v];
            min_index = v;
        }
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
    int pred[V] = {-1}; // Tableau avec les prédécesseurs dans le parcours

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
    if (u == src)
    {
        return u;
    }
    while (pred[u] != src && pred[u] != -1)
    {
        u = pred[u];
    }
    return u; // on retourne la prochaine tile sur laquelle le zombie doit aller
} 

// Calcul du sommet à partir de la position
int get_sommet(int x, int y, int hitboxWidth, int hitboxHeight)
{
    int tileWidth = 1920 / (MAP_WIDTH+2);
    int tileHeight = 1080 / (MAP_HEIGHT+3);
    int tileX = (x + hitboxWidth/2) / tileWidth - 1;
    int tileY = (y + 3*hitboxHeight/4) / tileHeight - 1;
    // Correction à cause de dépassement dans le mur
    if (tileX < 0)
    {
        tileX = 0;
    } else if (tileX > MAP_WIDTH-1)
    {
        tileX = MAP_WIDTH-1;
    }
    if (tileY < 0)
    {
        tileY = 0;
    } else if (tileY > MAP_HEIGHT-1)
    {
        tileY = MAP_HEIGHT-1;
    }
    return tileY * MAP_WIDTH + tileX;
}

// Pathfinding
void pathfinding(Enemy *zombie, Character *character, char *collisionTableFileName)
{
    int** map = readMapCollisionFile(collisionTableFileName);
    float** graph = mapToGraph(map);
    int zombieSommet = get_sommet(zombie->x, zombie->y, zombie->width, zombie->height);
    zombie->currentVertex = zombieSommet;
    int characterSommet = get_sommet(character->x, character->y, character->width, character->height);
    if (zombieSommet != characterSommet)
    {
        int nextVertex = dijkstra(graph, zombieSommet, characterSommet);
        zombie->nextVertex = nextVertex;   
    }

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

// On fait une fonction free custom pour pouvoir free la liste chaînée aussi
void free_zombie(Enemy *zombie)
{
    MedalList* current = zombie->medals;
    MedalList* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    SDL_DestroyTexture(zombie->texture);
    free(zombie->rectSrc);
    free(zombie);
}