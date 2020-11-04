#include <math.h>

#include <SDL2/SDL_opengl.h>

#include "include/life_drawer.h"


void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
{
    life_runner_init(&self->game, cells_x, cells_y);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        exit(1);
    }

    self->window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixels_x, pixels_y, SDL_WINDOW_OPENGL);
    
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

void life_drawer_redraw(const life_drawer* self)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);

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


void life_drawer_set_visual_cell(life_drawer* self, uint32_t cell_x, uint32_t cell_y, bool value)
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


void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value)
{
    pixel_x = (uint32_t)round(self->size_ratio_x * pixel_x);
    pixel_y = (uint32_t)round(self->size_ratio_y * pixel_y);

    if ((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < self->game.field->x_size) && (pixel_y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, pixel_x, pixel_y, value);
    }
}


uint64_t min(uint64_t num1, uint64_t num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
}


uint64_t max(uint64_t num1, uint64_t num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    return num2;
}


void life_drawer_draw_line(life_drawer* self, coordinates begin, coordinates end, bool value)
{
    int line_length_x = end.x - begin.x;
    int line_length_y = end.y - begin.y;
    int x = 0;
    int y = 0;
    if (line_length_y && (abs(line_length_y) > abs(line_length_x)))
    {
        double ratio = (double)line_length_x / (double)line_length_y;
        do
        {
            x = round((double)y * ratio);
            if (abs(x) > abs(line_length_x))
            {
                break;
            }
            life_drawer_change_cell(self, x + begin.x, y + begin.y, value);
            if (line_length_y > 0)
            {
                y++;
            }
            else
            {
                y--;
            }
        }
        while ((abs(y) < abs(line_length_y) + 1) && (abs(x) < abs(line_length_x) + 1));
    }
    else if (line_length_x && (abs(line_length_x) > abs(line_length_y)))
    {
        double ratio = (double)line_length_y / (double)line_length_x;
        do
        {
            y = round((double)x * ratio);
            life_drawer_change_cell(self, x + begin.x, y + begin.y, value);
            if (line_length_x > 0)
            {
                x++;
            }
            else
            {
                x--;
            }
        }
        while ((abs(x) < abs(line_length_x) + 1) && (abs(y) < abs(line_length_y) + 1));
    }
    else
    {
        life_drawer_change_cell(self, begin.x, begin.y, value);
    }
}

void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
