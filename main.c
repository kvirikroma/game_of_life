#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"
#include "include/life_runner.h"
#include "include/utils.h"
#include "include/life_drawer.h"
#include "include/key_handler.h"
#include "include/io_threader.h"

#define SAVEGAME_FILENAME "saved_game.life"


int main()
{
    uint32_t step_delay = 20;  // minimum is 20
    
    bool run = true;
    bool pause = true;
    bool lmb_pressed = false;
    bool rmb_pressed = false;
    uint8_t speed = 1;

    bool moved_once = false;
    direction movement;
    bool move = false;
    key_handler keyhandler;
    key_handler_init(&keyhandler, &pause, &movement, &move, &speed);

    io_threader threader;
    io_threader_init(&threader, 1600, 900, 512, 288, &lmb_pressed, &rmb_pressed, &move, &speed);

    SDL_Event event;
    while (run)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_c:
                    {
                        move = false;
                        moved_once = true;
                        lmb_pressed = false;
                        rmb_pressed = false;
                        bit_array2d* new_field = bit_array2d_init(threader.drawer.game.field->x_size, threader.drawer.game.field->y_size);
                        io_threader_lock_drawer(&threader);
                        bit_array2d_delete(threader.drawer.game.field);
                        threader.drawer.game.field = new_field;
                        io_threader_unlock_drawer(&threader);
                        break;
                    }
                    case SDLK_s:
                    {
                        io_threader_lock_drawer(&threader);
                        life_runner_snapshot snapshot = life_runner_to_snapshot(&threader.drawer.game);
                        io_threader_unlock_drawer(&threader);

                        save_runner_snapshot_to_file(&snapshot, SAVEGAME_FILENAME);
                        life_runner_snapshot_delete(&snapshot);
                        break;
                    }
                    case SDLK_l:
                    {
                        life_runner_snapshot file_snapshot;
                        if (load_runner_snapshot_from_file(&file_snapshot, SAVEGAME_FILENAME, false))
                        {
                            io_threader_lock_drawer(&threader);
                            life_runner_from_snapshot(&threader.drawer.game, file_snapshot, true);
                            io_threader_unlock_drawer(&threader);
                            life_runner_snapshot_delete(&file_snapshot);
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                key_handler_down(&keyhandler, event.key.keysym.sym);
            }
            if (event.type == SDL_KEYUP)
            {
                key_handler_up(&keyhandler, event.key.keysym.sym);
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    lmb_pressed = true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    rmb_pressed = true;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    lmb_pressed = false;
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    rmb_pressed = false;
                }
            }
        }

        if (move)
        {
            if ((!moved_once && keyhandler.pressed_keys.alt) || !keyhandler.pressed_keys.alt)
            {
                uint8_t distance = 1;
                if (keyhandler.pressed_keys.shift)
                {
                    distance = 4;
                }
                if (!lmb_pressed && !rmb_pressed && !keyhandler.pressed_keys.alt)
                {
                    distance *= 2;
                }
                io_threader_lock_drawer(&threader);
                life_runner_move_game(&threader.drawer.game, movement, distance);
                io_threader_unlock_drawer(&threader);
            }
            moved_once = true;
        }
        else
        {
            moved_once = false;
        }

        if (!lmb_pressed && !rmb_pressed)
        {
            if (!pause)
            {
                io_threader_lock_drawer(&threader);
                for (uint8_t step = 0; step < round(pow(1.4, speed)); step++)
                {
                    life_runner_make_step(&threader.drawer.game);
                    if (!(step % 4))
                    {
                        life_drawer_redraw(&threader.drawer);
                    }
                }
                io_threader_unlock_drawer(&threader);
            }
        }

        if (!move || moved_once)
        {
            int32_t delay = step_delay - (speed * 20);
            if (delay > 10)
            {
                sleep_ms(delay);
            }
            else
            {
                sleep_ms(10);
            }
            
        }
    }

    io_threader_delete(&threader);
    return 0;
}
