#ifndef LIFE_DRAWER_H
#define LIFE_DRAWER_H

#include <SDL2/SDL.h>

#include "life_runner.h"
#include "array2d.h"


typedef struct
{
    uint32_t pixels_x;
    uint32_t pixels_y;
    life_runner game;
    SDL_Window* window;
    array2d* visual_cells;
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
void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value);

#endif
