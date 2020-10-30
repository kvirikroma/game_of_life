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
    life_drawer_init(&drawer, 1600, 900, 64, 36);

    bit_array2d_set_bit(drawer.game.field, 2, 4, 1);
    bit_array2d_set_bit(drawer.game.field, 3, 4, 1);
    bit_array2d_set_bit(drawer.game.field, 4, 4, 1);

    bit_array2d_set_bit(drawer.game.field, 0, 0, 1);
    bit_array2d_set_bit(drawer.game.field, 1, 0, 1);
    bit_array2d_set_bit(drawer.game.field, 0, 1, 1);

    bit_array2d_set_bit(drawer.game.field, 15, 8, 1);
    bit_array2d_set_bit(drawer.game.field, 16, 9, 1);
    bit_array2d_set_bit(drawer.game.field, 16, 10, 1);
    bit_array2d_set_bit(drawer.game.field, 15, 10, 1);
    bit_array2d_set_bit(drawer.game.field, 14, 10, 1);
    
    bool run = true;
    SDL_Event event;
    while (run)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                run = false;
            }
        }
        life_drawer_redraw(&drawer);
        life_runner_make_step(&drawer.game);
        usleep(1000 * 100);
    }

    life_drawer_delete(&drawer);
    return 0;
}
