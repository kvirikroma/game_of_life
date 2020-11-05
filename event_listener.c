#include "include/event_listener.h"
#include "include/io_threader.h"
#include "include/utils.h"

#define SAVEGAME_FILENAME "saved_game.life"


void event_listener_init(event_listener* self)
{
    self->run = true;
    self->pause = true;
    self->lmb_pressed = false;
    self->rmb_pressed = false;
    self->moved_once = false;
    self->move = false;
    self->speed = 1;
    key_handler_init(&self->keyhandler, &self->pause, &self->movement, &self->move, &self->speed);
}

void event_listener_listen(event_listener* self, void* threader)
{
    io_threader* iothreader = (io_threader*)threader;
    while (SDL_PollEvent(&self->event))
    {
        if (self->event.type == SDL_QUIT)
        {
            self->run = false;
        }
        if (self->event.type == SDL_KEYDOWN)
        {
            switch (self->event.key.keysym.sym)
            {
                case SDLK_c:
                {
                    self->move = false;
                    self->moved_once = true;
                    self->lmb_pressed = false;
                    self->rmb_pressed = false;
                    bit_array2d* new_field = bit_array2d_init(iothreader->drawer.game.field->x_size, iothreader->drawer.game.field->y_size);
                    io_threader_lock_drawer(threader);
                    bit_array2d_delete(iothreader->drawer.game.field);
                    iothreader->drawer.game.field = new_field;
                    io_threader_unlock_drawer(threader);
                    break;
                }
                case SDLK_s:
                {
                    io_threader_lock_drawer(threader);
                    life_runner_snapshot snapshot = life_runner_to_snapshot(&iothreader->drawer.game);
                    io_threader_unlock_drawer(threader);

                    save_runner_snapshot_to_file(&snapshot, SAVEGAME_FILENAME);
                    life_runner_snapshot_delete(&snapshot);
                    break;
                }
                case SDLK_l:
                {
                    life_runner_snapshot file_snapshot;
                    if (load_runner_snapshot_from_file(&file_snapshot, SAVEGAME_FILENAME, false))
                    {
                        io_threader_lock_drawer(threader);
                        life_runner_from_snapshot(&iothreader->drawer.game, file_snapshot, true);
                        life_drawer_field_fit(&iothreader->drawer);
                        io_threader_unlock_drawer(threader);
                        life_runner_snapshot_delete(&file_snapshot);
                    }
                    break;
                }
                case SDLK_ESCAPE:
                {
                    self->run = false;
                }
                
                default:
                {
                    break;
                }
            }
            key_handler_down(&self->keyhandler, self->event.key.keysym.sym);
        }
        if (self->event.type == SDL_KEYUP)
        {
            key_handler_up(&self->keyhandler, self->event.key.keysym.sym);
        }
        if (self->event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (self->event.button.button == SDL_BUTTON_LEFT)
            {
                self->lmb_pressed = true;
            }
            if (self->event.button.button == SDL_BUTTON_RIGHT)
            {
                self->rmb_pressed = true;
            }
        }
        if (self->event.type == SDL_MOUSEBUTTONUP)
        {
            if (self->event.button.button == SDL_BUTTON_LEFT)
            {
                self->lmb_pressed = false;
            }
            if (self->event.button.button == SDL_BUTTON_RIGHT)
            {
                self->rmb_pressed = false;
            }
        }
    }
}


void event_listener_apply_movement(event_listener* self, void* threader, bool lock_drawer)
{
    io_threader* iothreader = (io_threader*)threader;
    if (self->move)
    {
        if ((!self->moved_once && self->keyhandler.pressed_keys.alt) || !self->keyhandler.pressed_keys.alt)
        {
            uint8_t distance = 1;
            if (self->keyhandler.pressed_keys.shift)
            {
                distance = 4;
            }
            if (!self->lmb_pressed && !self->rmb_pressed && !self->keyhandler.pressed_keys.alt)
            {
                distance *= 2;
            }
            if (lock_drawer)
            {
                io_threader_lock_drawer(iothreader);
                life_runner_move_game(&iothreader->drawer.game, self->movement, distance);
                io_threader_unlock_drawer(iothreader);
            }
            else
            {
                life_runner_move_game(&iothreader->drawer.game, self->movement, distance);
            }
        }
        self->moved_once = true;
    }
    else
    {
        self->moved_once = false;
    }
}
