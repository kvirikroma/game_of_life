#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"
#include "include/life_runner.h"
#include "include/utils.h"
#include "include/life_drawer.h"


int main(int argc, char** argv)
{
    life_runner game;
    life_runner_init(&game, 32, 18);

    bit_array2d_set_bit(game.field, 2, 4, 1);
    bit_array2d_set_bit(game.field, 3, 4, 1);
    bit_array2d_set_bit(game.field, 4, 4, 1);

    bit_array2d_set_bit(game.field, 0, 0, 1);
    bit_array2d_set_bit(game.field, 1, 0, 1);
    bit_array2d_set_bit(game.field, 0, 1, 1);

    for (uint32_t i = 0; i < 4; i++)
    {
        print_field(game.field);
        printf("\n");
        life_runner_make_step(&game);
        life_runner_move_game(&game, UP_LEFT, 2);
    }

    life_runner_delete(&game);
    return 0;
}
