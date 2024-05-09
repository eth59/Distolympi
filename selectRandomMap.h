int randomMapIndex();
char *getMapFromIndex(int index);
char *getColliderTable(int index);
char *getMapHoles(int index);
int randomInt(int max);
void getCoordFromTiles(int tileNumber, int *xTile, int *yTile);
int *convertirFichierEnTableau(const char *nomFichier, int *taille);
