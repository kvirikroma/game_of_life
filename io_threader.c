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
    pthread_mutex_init((pthread_mutex_t*)&self->drawer_lock, NULL);
    pthread_create((pthread_t*)&self->output_thread, NULL, output_thread_function, (void*)self);
}

void io_threader_delete(io_threader* self)
{
    self->stop_flag = true;
    pthread_join(self->output_thread, NULL);
}


void io_threader_lock_drawer(io_threader* self)
{
    pthread_mutex_lock((pthread_mutex_t*)&self->drawer_lock);
    sleep_ms(0.01);
}


void io_threader_unlock_drawer(io_threader* self)
{
    pthread_mutex_unlock((pthread_mutex_t*)&self->drawer_lock);
}


void* input_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    coordinates mouse = (coordinates){0, 0};
    while (!mouse.x && !mouse.y)
    {
        SDL_GetMouseState(&mouse.x, &mouse.y);
        sleep_ms(1);
    }
    while (!(const bool)self->stop_flag)
    {
        bool value = *self->lmb_pressed ^ *self->rmb_pressed;
        if (value)
        {
            SDL_GetMouseState(&mouse.x, &mouse.y);

            io_threader_lock_drawer(self);
            life_drawer_change_cell(&self->drawer, mouse.x, mouse.y, value);
            io_threader_unlock_drawer(self);
        }
        sleep_ms(0.001);
    }
    return 0;
}


void* output_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    life_drawer_init(&self->drawer, self->window_x, self->window_y, self->cells_x, self->cells_y);
    pthread_create((pthread_t*)&self->input_thread, NULL, input_thread_function, (void*)self);
    while (!(const bool)self->stop_flag)
    {
        io_threader_lock_drawer(self);
        life_drawer_redraw(&self->drawer);
        io_threader_unlock_drawer(self);

        SDL_UpdateWindowSurface(self->drawer.window);
        sleep_ms(2);
    }
    pthread_join(self->input_thread, NULL);
    pthread_mutex_destroy((pthread_mutex_t*)&self->drawer_lock);
    life_drawer_delete(&self->drawer);
    return 0;
}
