#include "include/life_drawer.h"


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
}
life_display_info;


life_display_info get_cell_size(coordinates window_size, coordinates game_size)
{
    life_display_info result;
    result.cell_size.x = (window_size.x / game_size.x);
    result.cell_size.y = (window_size.y / game_size.y);

    if (!result.cell_size.x)
    {
        result.cell_size.x = 1;
    }
    if (!result.cell_size.y)
    {
        result.cell_size.y = 1;
    }

    result.padding_top = (window_size.y - (game_size.y * result.cell_size.y)) / 2;
    if (result.cell_size.y == 1)
    {
        result.padding_top = 0;
    }
    result.padding_left = (window_size.x - (game_size.x * result.cell_size.x)) / 2;
    if (result.cell_size.x == 1)
    {
        result.padding_left = 0;
    }

    return result;
}


void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
{
    self->pixels_x = pixels_x;
    self->pixels_y = pixels_y;
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

    self->visual_cells = array2d_init(cells_x, cells_y, sizeof(SDL_Rect));
    life_display_info cells_info = get_cell_size((coordinates){pixels_x, pixels_y}, (coordinates){cells_x, cells_y});
    for (uint32_t y = 0; y < self->visual_cells->y_size; y++)
    {
        for (uint32_t x = 0; x < self->visual_cells->x_size; x++)
        {
            SDL_Rect* rect = (SDL_Rect*)array2d_get_item_ptr(self->visual_cells, x, y);
            rect->w = cells_info.cell_size.x;
            rect->h = cells_info.cell_size.y;
            rect->x = (cells_info.cell_size.x * x) + cells_info.padding_left;
            rect->y = (cells_info.cell_size.y * y) + cells_info.padding_top;
        }
    }
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

    for (uint32_t y = 0; y < self->visual_cells->y_size; y++)
    {
        for (uint32_t x = 0; x < self->visual_cells->x_size; x++)
        {
            SDL_Rect* rect = (SDL_Rect*)array2d_get_item_ptr(self->visual_cells, x, y);
            if (bit_array2d_get_bit(self->game.field, x, y, 0))
            {
                SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, 255, 255, 255));
            }
            else
            {
                SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
            }
        }
    }

    SDL_UpdateWindowSurface(self->window);
}

void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value)
{
    SDL_Rect* first_rect = (SDL_Rect*)array2d_get_item_ptr(self->visual_cells, 0, 0);
    pixel_x -= first_rect->x;
    pixel_y -= first_rect->y;

    life_display_info info = get_cell_size(
        (coordinates){self->pixels_x, self->pixels_y},
        (coordinates){self->game.field->x_size, self->game.field->y_size}
    );
    
    pixel_x /= info.cell_size.x;
    pixel_y /= info.cell_size.y;

    if ((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < self->game.field->x_size) && (pixel_y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, pixel_x, pixel_y, value);
    }
}

void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
