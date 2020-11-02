#include <math.h>

#include "include/life_drawer.h"


void life_drawer_init(volatile life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
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


void life_drawer_delete(volatile life_drawer* self)
{
    life_runner_delete(&self->game);
    SDL_DestroyWindow(self->window);
    SDL_Quit();
}

void life_drawer_redraw(volatile const life_drawer* self)
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
            uint32_t color = 0;
            if (bit_array2d_get_bit(self->game.field, bit_to_use.x, bit_to_use.y, 0))
            {
                color = 0xFFFFFFFF;
            }
            ((uint32_t*)screen_surface->pixels)[x + (screen_surface->w * y)] = color;
        }
    }
}


void life_drawer_set_visual_cell(volatile life_drawer* self, uint32_t cell_x, uint32_t cell_y, bool value)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    coordinates cell_size;
    cell_size.x = round((double)1 / self->size_ratio_x);
    cell_size.y = round((double)1 / self->size_ratio_y);
    for (uint32_t y = 0; y < cell_size.y; y++)
    {
        for (uint32_t x = 0; x < cell_size.x; x++)
        {
            uint32_t color = 0;
            if (value)
            {
                color = 0xFFFFFFFF;
            }
            ((uint32_t*)screen_surface->pixels)[(x + cell_x) + (screen_surface->w * (y + cell_y))] = color;
        }
    }
}


void life_drawer_change_cell(volatile life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value)
{
    pixel_x = (uint32_t)round(self->size_ratio_x * pixel_x);
    pixel_y = (uint32_t)round(self->size_ratio_y * pixel_y);

    if ((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < self->game.field->x_size) && (pixel_y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, pixel_x, pixel_y, value);
        SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
        SDL_Rect rect_to_fill;
        rect_to_fill.w = round((double)1 / self->size_ratio_x);
        rect_to_fill.h = round((double)1 / self->size_ratio_y);
        rect_to_fill.x = round(pixel_x / self->size_ratio_x);
        rect_to_fill.y = round(pixel_y / self->size_ratio_y);
        if (value)
        {
            SDL_FillRect(screen_surface, &rect_to_fill, SDL_MapRGB(screen_surface->format, 255, 255, 255));
        }
        else
        {
            SDL_FillRect(screen_surface, &rect_to_fill, SDL_MapRGB(screen_surface->format, 0, 0, 0));
        }
        SDL_UpdateWindowSurfaceRects(self->window, &rect_to_fill, 1);
    }
}

void life_drawer_change_window_size(volatile life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(volatile life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
