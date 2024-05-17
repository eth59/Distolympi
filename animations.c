#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void get_texture(SDL_Texture **texture, char *PATH_to_image, SDL_Renderer *renderer)
{
    SDL_Surface *imageSurface = IMG_Load(PATH_to_image);
    if (!imageSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error loading background: %s", IMG_GetError());
        exit(-1);
    }

    *texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error creating background texture: %s", SDL_GetError());
        exit(-1);
    }
}

//fonction de separation de tileset
void get_frames(SDL_Rect **rectSrc, int width_frame, int height_frame, int max_line_frame, int max_column_frame)
{
    int column_frame;
    int line_frame;
    int pointer_frame = 0;
    int x_pointer;
    int y_pointer;
    *rectSrc = (SDL_Rect*)malloc(sizeof(SDL_Rect) * max_line_frame * max_column_frame);
    
    for (line_frame = 0; line_frame < max_line_frame; line_frame++) {
        for (column_frame = 0; column_frame < max_column_frame; column_frame++) {
            x_pointer = width_frame * column_frame;
            y_pointer = height_frame * line_frame;
            (*rectSrc)[pointer_frame].x = x_pointer;
            (*rectSrc)[pointer_frame].y = y_pointer;
            (*rectSrc)[pointer_frame].w = width_frame;
            (*rectSrc)[pointer_frame].h = height_frame;
            pointer_frame++;
        }
    }
}