#ifndef LIFE_DRAWER_H
#define LIFE_DRAWER_H

#include <SDL2/SDL.h>

#include "life_runner.h"
#include "array2d.h"


typedef struct
{
    uint32_t x;
    uint32_t y;
}
coordinates;

typedef struct
{
    coordinates cell_size;
    uint32_t padding_top;
    uint32_t padding_left;
    bool square_cells;
}
life_display_info;

typedef struct
{
    uint32_t pixels_x;
    uint32_t pixels_y;
    life_runner game;
    SDL_Window* window;
    array2d* visual_cells;
    SDL_Rect padding_top;
    SDL_Rect padding_left;
    SDL_Rect padding_bottom;
    SDL_Rect padding_right;
    life_display_info window_config;
}
life_drawer;


// Init the life drawer and the game; open the window
void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y);

// Delete the life drawer and the game; close the window
void life_drawer_delete(life_drawer* self);

// Redraw the game
void life_drawer_redraw(life_drawer* self);

// Resize the game window
void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);

// Resize the game (number of cells)
void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);

// Change cell by coordinates of pixel
void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value, bool fast_redraw);

// Redraw the paddings on the screen
void life_drawer_redraw_paddings(life_drawer* self);

#endif
