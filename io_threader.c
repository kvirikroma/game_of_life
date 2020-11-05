#include "include/io_threader.h"
#include "include/utils.h"


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y)
{
    self->stop_flag = false;
    self->eventlistener.run = true;
    pthread_mutex_init((pthread_mutex_t*)&self->drawer_lock, NULL);
    output_thread_params* output_params = malloc(sizeof(output_thread_params));
    output_params->window_size = (coordinates){window_x, window_y};
    output_params->game_field_size = (coordinates){cells_x, cells_y};
    output_params->threader = self;
    pthread_create((pthread_t*)&self->output_thread, NULL, output_thread_function, (void*)output_params);
}

void io_threader_delete(io_threader* self)
{
    self->stop_flag = true;
    pthread_join(self->output_thread, NULL);
    pthread_mutex_destroy((pthread_mutex_t*)&self->drawer_lock);
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
    coordinates last_mouse_position;
    input_thread_params* params = (input_thread_params*)parameters;
    io_threader* self = params->threader;
    last_mouse_position = (coordinates){0, 0};
    event_listener_init(&self->eventlistener);
    while (!last_mouse_position.x && !last_mouse_position.y && !self->stop_flag)
    {
        event_listener_listen(&self->eventlistener, self);
        SDL_GetMouseState((int*)&last_mouse_position.x, (int*)&last_mouse_position.y);
        sleep_ms(1);
    }
    bool draw_line;
    int64_t last_move_time;
    int64_t new_move_time;
    while (!self->stop_flag)
    {
        event_listener_listen(&self->eventlistener, self);

        if (self->eventlistener.lmb_pressed ^ self->eventlistener.rmb_pressed)
        {
            coordinates new_mouse_position;
            SDL_GetMouseState((int*)&new_mouse_position.x, (int*)&new_mouse_position.y);

            io_threader_lock_drawer(self);
            if (draw_line)
            {
                life_drawer_draw_line(&self->drawer, last_mouse_position, new_mouse_position, self->eventlistener.lmb_pressed);
            }
            else
            {
                life_drawer_change_cell(&self->drawer, new_mouse_position.x, new_mouse_position.y, self->eventlistener.lmb_pressed);
                draw_line = true;
            }
            
            new_move_time = get_current_millisecond();
            if ((new_move_time - last_move_time) > 20)
            {
                event_listener_apply_movement(&self->eventlistener, self, false);
                last_move_time = new_move_time;
            }
            io_threader_unlock_drawer(self);

            last_mouse_position = new_mouse_position;
        }
        else
        {
            new_move_time = get_current_millisecond();
            if ((new_move_time - last_move_time) > 30)
            {
                bool pause_backup = self->eventlistener.pause;
                self->eventlistener.pause = true;
                event_listener_apply_movement(&self->eventlistener, self, true);
                self->eventlistener.pause = false;
                self->eventlistener.pause = pause_backup;
                last_move_time = new_move_time;
            }
            draw_line = false;
            sleep_ms(4);
        }
        sleep_ms(0.05);
    }
    free(parameters);
    return 0;
}


void* output_thread_function(void* parameters)
{
    output_thread_params* params = (output_thread_params*)parameters;
    io_threader* self = params->threader;
    input_thread_params* input_params = malloc(sizeof(input_thread_params));
    input_params->threader = self;
    life_drawer_init(&self->drawer, params->window_size.x, params->window_size.y, params->game_field_size.x, params->game_field_size.y);
    pthread_create((pthread_t*)&self->input_thread, NULL, input_thread_function, (void*)input_params);
    bit_array2d_set_bit(self->drawer.game.field, 1, 1, 1);

    while (!self->stop_flag)
    {
        if (!pthread_mutex_trylock(&self->drawer_lock))
        {
            life_drawer_redraw(&self->drawer);
            io_threader_unlock_drawer(self);
        }

        SDL_UpdateWindowSurface(self->drawer.window);
        sleep_ms(1);
    }
    pthread_join(self->input_thread, NULL);
    life_drawer_delete(&self->drawer);
    free(parameters);
    return 0;
}
