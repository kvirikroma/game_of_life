#ifndef LIFE_DRAWER_H
#define LIFE_DRAWER_H

#include <SDL2/SDL.h>

#include "life_runner.h"


typedef struct
{
    int32_t x;
    int32_t y;
}
coordinates;

typedef struct
{
    life_runner game;
    SDL_Window* window;
    double size_ratio_x;
    double size_ratio_y;
    double zoom_size_ratio_x;
    double zoom_size_ratio_y;
    uint32_t pixels_x, pixels_y;
}
life_drawer;


// Init the life drawer and the game; open the window
void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y);

// returns cell size in pixels
coordinates life_drawer_get_cell_size(const life_drawer* self);

// Returns true if the window ratio (pixels_x / pixels_y) is the same as the game ratio (cells_x/cells_y)
bool life_drawer_pixels_ratio_optimized(life_drawer* self);

// Delete the life drawer and the game; close the window
void life_drawer_delete(life_drawer* self);

// Redraw the game
void life_drawer_redraw(const life_drawer* self);

// Resize the game window
void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);

// Resize the game (number of cells)
void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);

// Change cell by coordinates of pixel
void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value);

// Draw line of cells or empty spaces on range of pixels
void life_drawer_draw_line(life_drawer* self, coordinates begin, coordinates end, bool value);

// Recalculate size ratio
void life_drawer_field_fit(life_drawer* self);

// Zoom in, follow mouse
void life_drawer_zoom_in(life_drawer* self, coordinates mouse);

// Zoom out, follow mouse
void life_drawer_zoom_out(life_drawer* self, coordinates mouse);

// Check if some zoom using at the moment
bool life_drawer_is_zoomed(const life_drawer* self);

// Get size of window
coordinates life_drawer_get_window_size(const life_drawer* self);

// Draw borders and grid when zoomed
void life_drawer_draw_zoom_layout(const life_drawer* self);

#endif
