#include <math.h>

#include "include/life_drawer.h"


void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
{
    life_runner_init(&self->game, cells_x, cells_y);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        exit(1);
    }

    self->window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixels_x, pixels_y, SDL_WINDOW_SHOWN);
    
    if (!self->window)
    {
        exit(1);
    }

    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(self->window);
    self->size_ratio_x = (double)cells_x / (double)pixels_x;
    self->size_ratio_y = (double)cells_y / (double)pixels_y;
}


void life_drawer_delete(life_drawer* self)
{
    life_runner_delete(&self->game);
    SDL_DestroyWindow(self->window);
    SDL_Quit();
}

void life_drawer_redraw(life_drawer* self)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));

    for (uint32_t y = 0; y < screen_surface->h; y++)
    {
        for (uint32_t x = 0; x < screen_surface->w; x++)
        {
            coordinates bit_to_use;
            bit_to_use.x = (uint32_t)round(self->size_ratio_x * x);
            bit_to_use.y = (uint32_t)round(self->size_ratio_y * y);
            uint32_t color;
            if (bit_array2d_get_bit(self->game.field, bit_to_use.x, bit_to_use.y, 0))
            {
                color = 0xFFFFFFFF;
            }
            else
            {
                color = 0;
            }
            ((uint32_t*)screen_surface->pixels)[x + (screen_surface->w * y)] = color;
        }
    }

    SDL_UpdateWindowSurface(self->window);
}

void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value)
{
    pixel_x = (uint32_t)round(self->size_ratio_x * pixel_x);
    pixel_y = (uint32_t)round(self->size_ratio_y * pixel_y);

    if ((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < self->game.field->x_size) && (pixel_y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, pixel_x, pixel_y, value);
    }
}

void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
