#include "include/io_threader.h"
#include "include/utils.h"


void io_threader_init(volatile io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, volatile bool* lmb_pressed, volatile bool* rmb_pressed, volatile bool* move)
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
    pthread_create((pthread_t*)&self->output_thread, NULL, output_thread_function, (void*)self);
}

void io_threader_delete(volatile io_threader* self)
{
    self->stop_flag = true;
    pthread_join(self->output_thread, NULL);
}


void io_threader_input_lock_drawer(volatile io_threader* self)
{
    while((const volatile bool)self->output_drawer_lock && !self->stop_flag);
    self->input_drawer_lock = true;
}


void io_threader_input_unlock_drawer(volatile io_threader* self)
{
    if ((const volatile bool)self->input_drawer_lock)
    {
        self->input_drawer_lock = false;
    }
}


void io_threader_output_lock_drawer(volatile io_threader* self)
{
    while((const volatile bool)self->input_drawer_lock && !self->stop_flag);
    self->output_drawer_lock = true;
}


void io_threader_output_unlock_drawer(volatile io_threader* self)
{
    if ((const volatile bool)self->output_drawer_lock)
    {
        self->output_drawer_lock = false;
    }
}


void* input_thread_function(void* parameters)
{
    volatile io_threader* self = (volatile io_threader*)parameters;
    while (!(const volatile bool)self->stop_flag)
    {
        if (*(const volatile bool*)self->lmb_pressed ^ *(const volatile bool*)self->rmb_pressed)
        {
            uint32_t x;
            uint32_t y;
            SDL_GetMouseState(&x, &y);
            io_threader_input_lock_drawer(self);
            if ((const volatile bool)self->stop_flag)
            {
                return 0;
            }
            life_drawer_change_cell(&self->drawer, x, y, *self->lmb_pressed);
            // if (*(const volatile bool*)self->move)
            // {
            //     io_threader_input_unlock_drawer(self);
            // }
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
    volatile io_threader* self = (volatile io_threader*)parameters;
    life_drawer_init(&self->drawer, self->window_x, self->window_y, self->cells_x, self->cells_y);
    pthread_create((pthread_t*)&self->input_thread, NULL, input_thread_function, (void*)self);
    while (!(const volatile bool)self->stop_flag)
    {
        io_threader_output_lock_drawer(self);
        if ((const volatile bool)self->stop_flag)
        {
            pthread_join(self->input_thread, NULL);
            life_drawer_delete(&self->drawer);
            return 0;
        }
        life_drawer_redraw(&self->drawer);
        io_threader_output_unlock_drawer(self);
        SDL_UpdateWindowSurface(self->drawer.window);
        sleep_ms(2);
    }
    pthread_join(self->input_thread, NULL);
    life_drawer_delete(&self->drawer);
    return 0;
}
