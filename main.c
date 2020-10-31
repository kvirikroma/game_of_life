#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/bit_array2d.h"
#include "include/array2d.h"
#include "include/life_runner.h"
#include "include/utils.h"
#include "include/life_drawer.h"


int main()
{
    life_drawer drawer;
    life_drawer_init(&drawer, 1600, 900, 512, 288);
    
    bool run = true;
    bool pause = true;
    bool lmb_pressed = false;
    bool rmb_pressed = false;
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
                    case SDLK_SPACE:
                    {
                        pause = !pause;
                        break;
                    }
                    
                    default:
                    {
                        break;
                    }
                }
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

        bool value_to_set;
        if (lmb_pressed)
        {
            value_to_set = 1;
        }
        if (rmb_pressed)
        {
            value_to_set = 0;
        }
        if (lmb_pressed || rmb_pressed)
        {
            uint32_t x;
            uint32_t y;
            SDL_GetMouseState(&x, &y);
            life_drawer_change_cell(&drawer, x, y, value_to_set);
            life_drawer_redraw(&drawer);
        }

        if (!pause && !lmb_pressed && !rmb_pressed)
        {
            life_runner_make_step(&drawer.game);
            life_drawer_redraw(&drawer);
            usleep(1000 * 64);
        }
        else
        {
            if (!lmb_pressed && !rmb_pressed)
            {
                usleep(1000 * 32);
            }
        }
    }

    life_drawer_delete(&drawer);
    return 0;
}
