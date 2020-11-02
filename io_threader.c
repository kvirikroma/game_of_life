#include "include/io_threader.h"
#include "include/utils.h"


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, bool* lmb_pressed, bool* rmb_pressed, bool* move)
{
    self->stop_flag = false;
    self->window_x = window_x;
    self->window_y = window_y;
    self->cells_x = cells_x;
    self->cells_y = cells_y;
    self->lmb_pressed = lmb_pressed;
    self->rmb_pressed = rmb_pressed;
    self->move = move;
    self->input_drawer_lock = false;
    self->output_drawer_lock = false;
    pthread_create(&self->output_thread, NULL, output_thread_function, self);
}

void io_threader_delete(io_threader* self)
{
    self->stop_flag = true;
    pthread_join(self->output_thread, NULL);
}


void io_threader_input_lock_drawer(io_threader* self)
{
    while((const volatile bool)self->output_drawer_lock && !self->stop_flag);
    self->input_drawer_lock = true;
}


void io_threader_input_unlock_drawer(io_threader* self)
{
    self->input_drawer_lock = false;
}


void io_threader_output_lock_drawer(io_threader* self)
{
    while((const volatile bool)self->input_drawer_lock && !self->stop_flag);
    self->output_drawer_lock = true;
}


void io_threader_output_unlock_drawer(io_threader* self)
{
    self->output_drawer_lock = false;
}


void* input_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    while (!(const volatile bool)self->stop_flag)
    {
        if (*self->lmb_pressed ^ *self->rmb_pressed)
        {
            uint32_t x;
            uint32_t y;
            SDL_GetMouseState(&x, &y);
            io_threader_input_lock_drawer(self);
            if (self->stop_flag)
            {
                return 0;
            }
            life_drawer_change_cell(&self->drawer, x, y, *self->lmb_pressed);
            if (*self->move)
            {
                io_threader_input_unlock_drawer(self);
            }
        }
        else
        {
            io_threader_input_unlock_drawer(self);
        }
    }
    return 0;
}


void* output_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    life_drawer_init(&self->drawer, self->window_x, self->window_y, self->cells_x, self->cells_y);
    pthread_create(&self->input_thread, NULL, input_thread_function, self);
    while (!(const volatile bool)self->stop_flag)
    {
        io_threader_output_lock_drawer(self);
        if (self->stop_flag)
        {
            pthread_join(self->input_thread, NULL);
            life_drawer_delete(&self->drawer);
            return 0;
        }
        life_drawer_redraw(&self->drawer);
        io_threader_output_unlock_drawer(self);
        SDL_UpdateWindowSurface(self->drawer.window);
        sleep_ms(5);
    }
    pthread_join(self->input_thread, NULL);
    life_drawer_delete(&self->drawer);
    return 0;
}
