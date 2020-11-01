#include "include/life_drawer.h"


life_display_info get_cell_size(coordinates window_size, coordinates game_size, bool square_cells)
{
    life_display_info result;
    result.cell_size.x = (window_size.x / game_size.x);
    result.cell_size.y = (window_size.y / game_size.y);
    if (square_cells)
    {
        if (result.cell_size.x < result.cell_size.y)
        {
            result.cell_size.y = result.cell_size.x;
        }
        else
        {
            result.cell_size.x = result.cell_size.y;
        }
    }

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


void configure_paddings(life_drawer* self)
{
    self->padding_left.w = self->padding_right.w = self->window_config.padding_left;
    self->padding_left.h = self->padding_right.h = self->pixels_y;
    self->padding_top.w = self->padding_bottom.w = self->pixels_x;
    self->padding_top.h = self->padding_bottom.h = self->window_config.padding_top;

    self->padding_left.x =
        self->padding_top.x =
        self->padding_left.y =
        self->padding_top.y =
        self->padding_right.y = 
        self->padding_bottom.x = 0;
    
    self->padding_right.x = self->window_config.padding_left + (self->window_config.cell_size.x * self->game.field->x_size);
    self->padding_bottom.y = self->window_config.padding_top + (self->window_config.cell_size.y * self->game.field->y_size);
}


void life_drawer_redraw_paddings(life_drawer* self)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    SDL_FillRect(screen_surface, &self->padding_top, SDL_MapRGB(screen_surface->format, 0xBB, 0xBB, 0xBB));
    SDL_FillRect(screen_surface, &self->padding_left, SDL_MapRGB(screen_surface->format, 0xBB, 0xBB, 0xBB));
    SDL_FillRect(screen_surface, &self->padding_right, SDL_MapRGB(screen_surface->format, 0xBB, 0xBB, 0xBB));
    SDL_FillRect(screen_surface, &self->padding_bottom, SDL_MapRGB(screen_surface->format, 0xBB, 0xBB, 0xBB));
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
    self->window_config = get_cell_size((coordinates){pixels_x, pixels_y}, (coordinates){cells_x, cells_y}, false);
    for (uint32_t y = 0; y < self->visual_cells->y_size; y++)
    {
        for (uint32_t x = 0; x < self->visual_cells->x_size; x++)
        {
            SDL_Rect* rect = (SDL_Rect*)array2d_get_item_ptr(self->visual_cells, x, y);
            rect->w = self->window_config.cell_size.x;
            rect->h = self->window_config.cell_size.y;
            rect->x = (self->window_config.cell_size.x * x) + self->window_config.padding_left;
            rect->y = (self->window_config.cell_size.y * y) + self->window_config.padding_top;
        }
    }

    configure_paddings(self);
    life_drawer_redraw_paddings(self);
    SDL_UpdateWindowSurface(self->window);
}


void life_drawer_delete(life_drawer* self)
{
    array2d_delete(self->visual_cells);
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

    life_drawer_redraw_paddings(self);
    SDL_UpdateWindowSurface(self->window);
}

void life_drawer_change_cell(life_drawer* self, uint32_t pixel_x, uint32_t pixel_y, bool value, bool fast_redraw)
{
    SDL_Rect* first_rect = (SDL_Rect*)array2d_get_item_ptr(self->visual_cells, 0, 0);
    pixel_x -= first_rect->x;
    pixel_y -= first_rect->y;
    
    pixel_x /= self->window_config.cell_size.x;
    pixel_y /= self->window_config.cell_size.y;

    if ((pixel_x >= 0) && (pixel_y >= 0) && (pixel_x < self->game.field->x_size) && (pixel_y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, pixel_x, pixel_y, value);
        if (fast_redraw)
        {
            SDL_Rect* rect = array2d_get_item_ptr(self->visual_cells, pixel_x, pixel_y);
            SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
            if (value)
            {
                SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, 255, 255, 255));
            }
            else
            {
                SDL_FillRect(screen_surface, rect, SDL_MapRGB(screen_surface->format, 0, 0, 0));
            }
            SDL_UpdateWindowSurface(self->window);
        }
    }
}

void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
