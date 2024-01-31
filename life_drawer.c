#include <math.h>

#include "include/life_drawer.h"

#define ZOOM_BORDERS_SIZE  6
#define ZOOM_LAYOUT_COLOR  0x22888888
#define GRID_MIN_CELL_SIZE 18
#define ZOOM_UNIT_SIZE 1.15


typedef enum
{
    ZOOM_IN,
    ZOOM_OUT
}
zoom;


coordinates life_drawer_get_cell_size(const life_drawer* self)
{
    return (coordinates){
        round((double)1 / self->zoom_size_ratio_x),
        round((double)1 / self->zoom_size_ratio_y)
    };
}


// in game field
static coordinates get_cell_coordinates(const life_drawer* self, coordinates pixel_position)
{
    return (coordinates){
        round(self->zoom_size_ratio_x * pixel_position.x),
        round(self->zoom_size_ratio_y * pixel_position.y)
    };
}


static bool is_pixel_on_grid(const life_drawer* self, coordinates pixel)
{
    if (!pixel.x || !pixel.y)
    {
        return false;
    }
    coordinates cell_current;
    coordinates cell_previous;
    cell_current = get_cell_coordinates(self, (coordinates){pixel.x, 0});
    cell_previous = get_cell_coordinates(self, (coordinates){pixel.x - 1, 0});
    if (cell_current.x != cell_previous.x)
    {
        return true;
    }
    cell_current = get_cell_coordinates(self, (coordinates){0, pixel.y});
    cell_previous = get_cell_coordinates(self, (coordinates){0, pixel.y - 1});
    if (cell_current.x != cell_previous.x)
    {
        return true;
    }
    return false;
}


// on screen
/*static coordinates get_cell_position(const life_drawer* self, coordinates cell)
{
    return (coordinates){
        round((double)cell.x / self->zoom_size_ratio_x),
        round((double)cell.y / self->zoom_size_ratio_y)
    };
}*/


void life_drawer_init(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y, uint32_t cells_x, uint32_t cells_y)
{
    life_runner_init(&self->game, cells_x, cells_y);

    if (SDL_Init(SDL_INIT_VIDEO))
    {
        exit(1);
    }

    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE);
    self->window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pixels_x, pixels_y, SDL_WINDOW_SHOWN);
    
    if (!self->window)
    {
        exit(1);
    }

    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));
    SDL_UpdateWindowSurface(self->window);
    life_drawer_field_fit(self);
}


void life_drawer_field_fit(life_drawer* self)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    self->size_ratio_x = (double)self->game.field->x_size / (double)screen_surface->w;
    self->size_ratio_y = (double)self->game.field->y_size / (double)screen_surface->h;
    self->zoom_size_ratio_x = self->size_ratio_x;
    self->zoom_size_ratio_y = self->size_ratio_y;
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
            if (life_drawer_is_zoomed(self))
            {
                if ((x < ZOOM_BORDERS_SIZE) || (x > (screen_surface->w - ZOOM_BORDERS_SIZE)))
                {
                    continue;
                }
                if ((y < ZOOM_BORDERS_SIZE) || (y > (screen_surface->h - ZOOM_BORDERS_SIZE)))
                {
                    continue;
                }
                coordinates cell_size = life_drawer_get_cell_size(self);
                if ((cell_size.x >= GRID_MIN_CELL_SIZE) && (cell_size.x >= GRID_MIN_CELL_SIZE))
                {
                    if (is_pixel_on_grid(self, (coordinates){x, y}))
                    {
                        continue;
                    }
                }
            }
            coordinates bit_to_use;
            bit_to_use.x = (uint32_t)round(self->zoom_size_ratio_x * x);
            bit_to_use.y = (uint32_t)round(self->zoom_size_ratio_y * y);
            uint32_t color = 0;
            if (bit_array2d_get_bit(self->game.field, bit_to_use.x, bit_to_use.y))
            {
                color = 0xFFFFFFFF;
            }
            ((uint32_t*)screen_surface->pixels)[x + (screen_surface->w * y)] = color;
        }
    }
}


void life_drawer_draw_zoom_layout(const life_drawer* self)
{
    if (!life_drawer_is_zoomed(self))
    {
        return;
    }
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    SDL_Rect borders[4];
    borders[0].x = borders[1].x = borders[0].y = borders[1].y = borders[2].y = borders[3].x = 0;
    borders[2].x = screen_surface->w - ZOOM_BORDERS_SIZE;
    borders[3].y = screen_surface->h - ZOOM_BORDERS_SIZE;
    borders[0].w = borders[1].h = borders[2].w = borders[3].h = ZOOM_BORDERS_SIZE;
    borders[0].h = borders[2].h = screen_surface->h;
    borders[1].w = borders[3].w = screen_surface->w;
    SDL_FillRects(screen_surface, borders, 4, ZOOM_LAYOUT_COLOR);
    coordinates cell_size = life_drawer_get_cell_size(self);
    if ((cell_size.x < GRID_MIN_CELL_SIZE) || (cell_size.x < GRID_MIN_CELL_SIZE))
    {
        return;
    }
    coordinates window_size = life_drawer_get_window_size(self);
    for (uint32_t pixel_x = 1; pixel_x < window_size.x; pixel_x++)
    {
        coordinates cell_current = get_cell_coordinates(self, (coordinates){pixel_x, 0});
        coordinates cell_previous = get_cell_coordinates(self, (coordinates){pixel_x - 1, 0});
        if (cell_current.x != cell_previous.x)
        {
            SDL_Rect line;
            line.x = pixel_x;
            line.y = 0;
            line.h = window_size.y;
            line.w = 1;
            SDL_FillRect(screen_surface, &line, ZOOM_LAYOUT_COLOR);
        }
    }
    for (uint32_t pixel_y = 1; pixel_y < window_size.y; pixel_y++)
    {
        coordinates cell_current = get_cell_coordinates(self, (coordinates){0, pixel_y});
        coordinates cell_previous = get_cell_coordinates(self, (coordinates){0, pixel_y - 1});
        if (cell_current.y != cell_previous.y)
        {
            SDL_Rect line;
            line.y = pixel_y;
            line.x = 0;
            line.w = window_size.x;
            line.h = 1;
            SDL_FillRect(screen_surface, &line, ZOOM_LAYOUT_COLOR);
        }
    }
}


void life_drawer_set_visual_cell(life_drawer* self, uint32_t cell_x, uint32_t cell_y, bool value)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    coordinates cell_size = life_drawer_get_cell_size(self);
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
    coordinates cell = get_cell_coordinates(self, (coordinates){pixel_x, pixel_y});
    if ((cell.x >= 0) && (cell.y >= 0) && (cell.x < self->game.field->x_size) && (cell.y < self->game.field->y_size))
    {
        bit_array2d_set_bit(self->game.field, cell.x, cell.y, value);
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
    else if (line_length_x && (abs(line_length_x) >= abs(line_length_y)))
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

bool life_drawer_is_zoomed(const life_drawer* self)
{
    return (
        (self->size_ratio_x != self->zoom_size_ratio_x) || 
        (self->size_ratio_y != self->zoom_size_ratio_y)
    );
}

coordinates life_drawer_get_window_size(const life_drawer* self)
{
    SDL_Surface* screen_surface = SDL_GetWindowSurface(self->window);
    return (coordinates){screen_surface->w, screen_surface->h};
}

static void life_drawer_zoom(life_drawer* self, coordinates mouse, zoom method)
{
    coordinates cell_under_mouse_start = get_cell_coordinates(self, mouse);
    if (method == ZOOM_OUT)
    {
        if ((self->zoom_size_ratio_x > self->size_ratio_x) || (self->zoom_size_ratio_y > self->size_ratio_y))
        {
            self->zoom_size_ratio_x = self->size_ratio_x;
            self->zoom_size_ratio_y = self->size_ratio_y;
        }
        if ((self->zoom_size_ratio_x >= self->size_ratio_x) || (self->zoom_size_ratio_y >= self->size_ratio_y))
        {
            return;
        }
        self->zoom_size_ratio_x *= ZOOM_UNIT_SIZE;
        self->zoom_size_ratio_y *= ZOOM_UNIT_SIZE;
    }
    else
    {
        coordinates window_size = life_drawer_get_window_size(self);
        coordinates cell_size = life_drawer_get_cell_size(self);
        if ((cell_size.x * 8 >= window_size.x) || (cell_size.y * 8 >= window_size.y))
        {
            return;
        }
        self->zoom_size_ratio_x /= ZOOM_UNIT_SIZE;
        self->zoom_size_ratio_y /= ZOOM_UNIT_SIZE;
    }
    coordinates cell_under_mouse_end = get_cell_coordinates(self, mouse);
    int32_t cell_offset_x = cell_under_mouse_end.x - cell_under_mouse_start.x;
    int32_t cell_offset_y = cell_under_mouse_end.y - cell_under_mouse_start.y;
    if (cell_offset_x)
    {
        if (cell_offset_x > 0)
        {
            life_runner_move_game(&self->game, RIGHT, cell_offset_x);
        }
        else
        {
            life_runner_move_game(&self->game, LEFT, -cell_offset_x);
        }
        
    }
    if (cell_offset_y)
    {
        if (cell_offset_y > 0)
        {
            life_runner_move_game(&self->game, DOWN, cell_offset_y);
        }
        else
        {
            life_runner_move_game(&self->game, UP, -cell_offset_y);
        }
    }
}

void life_drawer_zoom_in(life_drawer* self, coordinates mouse)
{
    life_drawer_zoom(self, mouse, ZOOM_IN);
}

void life_drawer_zoom_out(life_drawer* self, coordinates mouse)
{
    life_drawer_zoom(self, mouse, ZOOM_OUT);
}

void life_drawer_change_window_size(life_drawer* self, uint32_t pixels_x, uint32_t pixels_y);  // TODO

void life_drawer_change_game_size(life_drawer* self, uint32_t cells_x, uint32_t cells_y);  // TODO
