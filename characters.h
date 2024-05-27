#define PI 3.14159265358979323846264338327950288 

void moveCharacter(Character *character, SDL_DisplayMode displayMode, float dx, float dy, char *collisionTable);
int get_direction_and_move(int key_up_pressed ,int key_down_pressed,int key_left_pressed,int key_right_pressed,Character *character, SDL_DisplayMode displayMode, char *collisionTable);
SDL_Rect get_Rectdest_attacks(int destination,Character character);
int get_melee_direction(int character_x,int character_y,int mouse_x,int mouse_y);
int get_CollisionInd(int key_up_pressed ,int key_down_pressed,int key_left_pressed,int key_right_pressed,Character *character,char* collisionTable);
    