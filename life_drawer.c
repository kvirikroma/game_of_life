#include "include/life_drawer.h"


life_drawer life_drawer_init(uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
{
    life_drawer result;
    result.cells_x = cells_x;
    result.cells_y = cells_y;
    result.pixels_x = pixels_x;
    result.pixels_y = pixels_y;
    life_runner_init(&result.game, cells_x, cells_y);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        exit(1);
    }

    result.window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixels_x, pixels_y, SDL_WINDOW_SHOWN);
    
    if (!result.window) {
        exit(1);
    }

    SDL_Surface* screen_surface = SDL_GetWindowSurface(result.window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(result.window);

    return result;
}

void life_drawer_delete(life_drawer* drawer);

void life_drawer_redraw(life_drawer* drawer);

void life_drawer_change_window_size(life_drawer* drawer, uint32_t pixels_x, uint32_t pixels_y);

void life_drawer_change_game_size(life_drawer* drawer, uint32_t cells_x, uint32_t cells_y);
