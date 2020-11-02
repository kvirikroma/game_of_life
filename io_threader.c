#include "include/io_threader.h"
#include "include/utils.h"


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, bool* lmb_pressed, bool* rmb_pressed, bool* move)
{
    self->stop_flag = false;
    self->window_x = window_x;
    self->window_y = window_y;
    self->cells_x = cells_x;
    self->cells_y = cells_y;
    self->output_pause = false;
    self->lmb_pressed = lmb_pressed;
    self->rmb_pressed = rmb_pressed;
    self->move = move;
    pthread_create(&self->output_thread, NULL, output_thread_function, self);
}

void io_threader_delete(io_threader* self)
{
    self->stop_flag = true;
    pthread_join(self->output_thread, NULL);
}


void io_threader_output_pause(io_threader* self)
{
    self->output_pause = true;
    while(!self->output_pause_response);
}


void io_threader_output_unpause(io_threader* self)
{
    self->output_pause = false;
}


void io_threader_input_pause(io_threader* self)
{
    self->input_pause = true;
    while(!self->input_pause_response);
}


void io_threader_input_unpause(io_threader* self)
{
    self->input_pause = false;
}


void* input_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    while (!self->stop_flag)
    {
        if (!self->input_pause)
        {
            self->input_pause_response = false;
            if (*self->lmb_pressed ^ *self->rmb_pressed)
            {
                uint32_t x;
                uint32_t y;
                SDL_GetMouseState(&x, &y);
                io_threader_output_pause(self);
                life_drawer_change_cell(&self->drawer, x, y, *self->lmb_pressed);
                if (*self->move)
                {
                    io_threader_output_unpause(self);
                }
            }
        }
        else
        {
            io_threader_output_unpause(self);
            self->input_pause_response = true;
            sleep_ms(2);
        }
        sleep_ms(1);
    }
    return 0;
}


void* output_thread_function(void* parameters)
{
    io_threader* self = (io_threader*)parameters;
    life_drawer_init(&self->drawer, self->window_x, self->window_y, self->cells_x, self->cells_y);
    pthread_create(&self->input_thread, NULL, input_thread_function, self);
    while (!self->stop_flag)
    {
        if (!self->output_pause)
        {
            self->output_pause_response = false;
            life_drawer_redraw(&self->drawer);
        }
        else
        {
            self->output_pause_response = true;
        }
        sleep_ms(5);
        SDL_UpdateWindowSurface(self->drawer.window);
    }
    pthread_join(self->input_thread, NULL);
    life_drawer_delete(&self->drawer);
    return 0;
}
