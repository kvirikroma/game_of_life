#include "include/io_threader.h"
#include "include/utils.h"


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y)
{
    self->stop_flag = false;
    self->input.run = true;
    self->threads_started = false;
    self->redrawed = false;
    self->mouse_inited = false;
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
    sleep_ms(0.001);
}


void io_threader_unlock_drawer(io_threader* self)
{
    pthread_mutex_unlock((pthread_mutex_t*)&self->drawer_lock);
    sleep_ms(0.001);
}


void* input_thread_function(void* parameters)
{
    coordinates last_mouse_position;
    input_thread_params* params = (input_thread_params*)parameters;
    io_threader* self = params->threader;
    last_mouse_position = (coordinates){0, 0};
    event_listener_init(&self->input);
    self->threads_started = true;
    bool draw_line = false;
    int64_t last_move_time = 0;
    int64_t new_move_time;
    while (!self->stop_flag)
    {
        event_listener_listen(&self->input, self);
        if (!self->mouse_inited)
        {
            SDL_GetMouseState((int*)&last_mouse_position.x, (int*)&last_mouse_position.y);
            if (last_mouse_position.x || last_mouse_position.y)
            {
                self->mouse_inited = true;
            }
            else
            {
                continue;
            }
        }
        if ((self->input.lmb_pressed ^ self->input.rmb_pressed) || self->input.mmb_pressed)
        {
            coordinates new_mouse_position;
            SDL_GetMouseState((int*)&new_mouse_position.x, (int*)&new_mouse_position.y);

            io_threader_lock_drawer(self);
            if (draw_line && !self->input.mmb_pressed)
            {
                life_drawer_draw_line(&self->drawer, last_mouse_position, new_mouse_position, self->input.lmb_pressed);
            }
            else if (self->input.mmb_pressed)
            {
                coordinates cell_size = life_drawer_get_cell_size(&self->drawer);
                life_runner_move_game_by_coordinates(
                    &self->drawer.game,
                    round((double)(new_mouse_position.x / cell_size.x - last_mouse_position.x / cell_size.x)),
                    round((double)(new_mouse_position.y / cell_size.y - last_mouse_position.y / cell_size.y))
                );
            }
            else
            {
                life_drawer_change_cell(&self->drawer, new_mouse_position.x, new_mouse_position.y, self->input.lmb_pressed);
                draw_line = true;
            }
            self->redrawed = false;
            
            new_move_time = get_current_millisecond();
            if (((new_move_time - last_move_time) > 1) && !self->input.mmb_pressed)
            {
                event_listener_apply_movement(&self->input, self, false);
                last_move_time = new_move_time;
            }

            io_threader_unlock_drawer(self);

            last_mouse_position = new_mouse_position;
        }
        else
        {
            new_move_time = get_current_millisecond();
            if ((new_move_time - last_move_time) > 25)
            {
                event_listener_apply_movement(&self->input, self, true);
                last_move_time = new_move_time;
            }
            draw_line = false;
            sleep_ms(0.1);
        }
        sleep_ms(0.2);
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
    int64_t last_activity = get_current_millisecond();

    while (!self->stop_flag)
    {
        
        bool was_redrawed_before = self->redrawed;
        if (!was_redrawed_before)
        {
            io_threader_lock_drawer(self);
            life_drawer_redraw(&self->drawer);
            life_drawer_draw_zoom_layout(&self->drawer);
            self->redrawed = true;
            io_threader_unlock_drawer(self);
            last_activity = get_current_millisecond();
        }
        if (!was_redrawed_before)
        {
            SDL_UpdateWindowSurface(self->drawer.window);
        }
        if ((get_current_millisecond() - last_activity) > 500)
        {
            sleep_ms(55);
            SDL_UpdateWindowSurface(self->drawer.window);
            sleep_ms(55);
        }
        sleep_ms(2);
    }
    pthread_join(self->input_thread, NULL);
    life_drawer_delete(&self->drawer);
    free(parameters);
    return 0;
}
