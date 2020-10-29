#include "include/life_drawer.h"


life_drawer life_drawer_init(uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y);

void life_drawer_delete(life_drawer* drawer);

void life_drawer_redraw(life_drawer* drawer);

void life_drawer_change_window_size(life_drawer* drawer, uint32_t pixels_x, uint32_t pixels_y);

void life_drawer_change_game_size(life_drawer* drawer, uint32_t cells_x, uint32_t cells_y);

void life_drawer_move_game(direction move_direction, uint32_t distance);
