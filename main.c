#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"
#include "include/life_runner.h"
#include "include/utils.h"
#include "include/life_drawer.h"
#include "include/key_handlers.h"


#define PIXELS_PER_MS 8000


int main()
{
    life_drawer drawer;
    life_drawer_init(&drawer, 1600, 900, 512, 288);
    uint32_t game_slowdown = 40;
    
    bool run = true;
    bool pause = true;
    bool lmb_pressed = false;
    bool rmb_pressed = false;

    bool moved_once = false;
    direction movement;
    bool move = false;

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
                keydown_handler(event.key.keysym.sym, &pause, &movement, &move);
            }
            if (event.type == SDL_KEYUP)
            {
                keyup_handler(event.key.keysym.sym, &pause, &movement, &move);
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

        if (lmb_pressed ^ rmb_pressed)
        {
            uint32_t x;
            uint32_t y;
            SDL_GetMouseState(&x, &y);
            life_drawer_change_cell(&drawer, x, y, lmb_pressed);
            life_drawer_redraw(&drawer);
        }

        if (move)
        {
            if ((!moved_once && pressed_keys.alt) || !pressed_keys.alt)
            {
                uint8_t distance = 1;
                if (pressed_keys.shift)
                {
                    distance = 4;
                }
                if (!lmb_pressed && !rmb_pressed && !pressed_keys.alt)
                {
                    distance *= 2;
                }
                life_runner_move_game(&drawer.game, movement, distance);
                life_drawer_redraw(&drawer);
            }
            moved_once = true;
        }
        else
        {
            moved_once = false;
        }

        if (!lmb_pressed && !rmb_pressed)
        {
            int16_t ms = game_slowdown;
            if (!pause)
            {
                life_runner_make_step(&drawer.game);
                if (!move || moved_once)
                {
                    life_drawer_redraw(&drawer);
                    ms -= (drawer.game.field->x_size * drawer.game.field->y_size) / PIXELS_PER_MS;
                    if (ms < 0)
                    {
                        ms = 0;
                    }
                }
            }
            sleep_ms(ms);
        }
    }

    life_drawer_delete(&drawer);
    return 0;
}
