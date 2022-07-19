#include "include/event_listener.h"
#include "include/io_threader.h"
#include "include/utils.h"

#define SAVEGAME_FILENAME "saved_game.life"
#define MAX_EVENTS_AT_A_TIME 4


void event_listener_init(event_listener* self)
{
    self->run = true;
    self->pause = true;
    self->lmb_pressed = false;
    self->rmb_pressed = false;
    self->moved_once = false;
    self->move = false;
    self->speed = 1;
    self->zoom_in = false;
    self->zoom_out = false;
    key_handler_init(&self->keyhandler, &self->pause, &self->movement, &self->move, &self->speed);
}

void event_listener_listen(event_listener* self, void* threader)
{
    io_threader* iothreader = (io_threader*)threader;
    bool flush_event = false;
    for (uint8_t i = 0; i < MAX_EVENTS_AT_A_TIME; i++)
    {
        if (!SDL_PollEvent(&self->event))
        {
            break;
        }
        switch (self->event.type)
        {
            case SDL_QUIT:
            {
                self->run = false;
                break;
            }
            case SDL_KEYUP:
            {
                key_handler_up(&self->keyhandler, self->event.key.keysym.sym);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (self->event.button.button == SDL_BUTTON_LEFT)
                {
                    self->lmb_pressed = true;
                }
                if (self->event.button.button == SDL_BUTTON_RIGHT)
                {
                    self->rmb_pressed = true;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (self->event.button.button == SDL_BUTTON_LEFT)
                {
                    self->lmb_pressed = false;
                }
                if (self->event.button.button == SDL_BUTTON_RIGHT)
                {
                    self->rmb_pressed = false;
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (self->event.key.keysym.sym)
                {
                    case SDLK_c:
                    {
                        self->speed = 1;
                        self->move = false;
                        self->pause = true;
                        self->moved_once = true;
                        self->lmb_pressed = false;
                        self->rmb_pressed = false;

                        io_threader_lock_drawer(threader);
                        bit_array2d_erase(iothreader->drawer.game.field);
                        iothreader->redrawed = false;
                        io_threader_unlock_drawer(threader);
                        flush_event = true;
                        break;
                    }
                    case SDLK_s:
                    {
                        io_threader_lock_drawer(threader);
                        life_runner_snapshot snapshot = life_runner_to_snapshot(&iothreader->drawer.game);
                        io_threader_unlock_drawer(threader);

                        save_runner_snapshot_to_file(&snapshot, SAVEGAME_FILENAME);
                        life_runner_snapshot_delete(&snapshot);
                        flush_event = true;
                        break;
                    }
                    case SDLK_l:
                    {
                        life_runner_snapshot file_snapshot;
                        if (load_runner_snapshot_from_file(&file_snapshot, SAVEGAME_FILENAME, false))
                        {
                            self->speed = 1;
                            self->pause = true;

                            io_threader_lock_drawer(threader);
                            life_runner_from_snapshot(&iothreader->drawer.game, file_snapshot, true);
                            life_drawer_field_fit(&iothreader->drawer);
                            iothreader->redrawed = false;
                            io_threader_unlock_drawer(threader);

                            life_runner_snapshot_delete(&file_snapshot);
                            flush_event = true;
                        }
                        break;
                    }
                    case SDLK_0:
                    {
                        self->speed = 0;
                        self->pause = true;

                        io_threader_lock_drawer(threader);
                        life_runner_make_step(&iothreader->drawer.game);
                        iothreader->redrawed = false;
                        io_threader_unlock_drawer(threader);
                        flush_event = true;
                        break;
                    }
                    case SDLK_ESCAPE:
                    {
                        self->run = false;
                        flush_event = true;
                        break;
                    }

                    default:
                    {
                        if (key_handler_down(&self->keyhandler, self->event.key.keysym.sym))
                        {
                            flush_event = true;
                        }
                    }
                }
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                if (self->event.wheel.y > 0)
                {
                    self->zoom_in = true;
                }
                if (self->event.wheel.y < 0)
                {
                    self->zoom_out = true;
                }
            }
        }
    }
    if (flush_event)
    {
        SDL_FlushEvent(SDL_KEYDOWN);
    }
}


static void scale_zoom(event_listener* self, io_threader* threader)
{
    if (!threader->mouse_inited)
    {
        return;
    }
    coordinates mouse = (coordinates){0, 0};
    SDL_GetMouseState((int*)&mouse.x, (int*)&mouse.y);
    if (self->zoom_in)
    {
        life_drawer_zoom_in(&threader->drawer, mouse);
        self->zoom_in = false;
        threader->redrawed = false;
    }
    if (self->zoom_out)
    {
        life_drawer_zoom_out(&threader->drawer, mouse);
        self->zoom_out = false;
        threader->redrawed = false;
    }
}


void event_listener_apply_movement(event_listener* self, void* threader, bool lock_drawer)
{
    io_threader* iothreader = (io_threader*)threader;
    bool drawer_locked = false;
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
                if (!iothreader->input.pause)
                {
                    distance *= iothreader->input.speed;
                }
            }
            if (lock_drawer)
            {
                io_threader_lock_drawer(iothreader);
                drawer_locked = true;
            }
            life_runner_move_game(&iothreader->drawer.game, self->movement, distance);
        }
        self->moved_once = true;
    }
    else
    {
        self->moved_once = false;
    }
    if (lock_drawer && !drawer_locked && (self->zoom_out || self->zoom_in))
    {
        io_threader_lock_drawer(iothreader);
        drawer_locked = true;
    }
    scale_zoom(self, iothreader);
    iothreader->redrawed = false;
    if (lock_drawer && drawer_locked)
    {
        io_threader_unlock_drawer(iothreader);
    }
}
