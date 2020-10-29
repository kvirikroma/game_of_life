#ifndef LIFE_DRAWER_H
#define LIFE_DRAWER_H

#include <SDL2/SDL.h>

#include "life_runner.h"


typedef struct
{
    uint32_t pixels_x;
    uint32_t pixels_y;
    uint32_t cells_x;
    uint32_t cells_y;
    life_runner game;
    SDL_Window* window;
}
life_drawer;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
}
direction;


// Init the life drawer and the game; open the window
life_drawer life_drawer_init(uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y);

// Delete the life drawer and the game; close the window
void life_drawer_delete(life_drawer* drawer);

// Redraw the game
void life_drawer_redraw(life_drawer* drawer);

// Resize the game window
void life_drawer_change_window_size(life_drawer* drawer, uint32_t pixels_x, uint32_t pixels_y);

// Resize the game (number of cells)
void life_drawer_change_game_size(life_drawer* drawer, uint32_t cells_x, uint32_t cells_y);

// Move the game in window (distance is in cells)
void life_drawer_move_game(direction move_direction, uint32_t distance);


#endif
