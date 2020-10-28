#include <stdio.h>
#include <stdlib.h>

#include "include/bit_array2d.h"
#include "include/life_runner.h"


void exit_on_syntax_error(void)
{
    fprintf(stderr, "%s", "Invalid command line parameters detected!\n");
    exit(1);
}


void* check_pointer_after_malloc(void* ptr)
{
    if (!ptr)
    {
        fprintf(stderr, "%s", "An error occurred while allocating memory! Exiting\n");
        exit(1);
    }
    return ptr;
}


void print_field(bit_array2d* field)
{
    for (uint32_t y = 0; y < field->y_size; y++)
    {
        for (uint32_t x = 0; x < field->x_size; x++)
        {
            printf("%d ", bit_array2d_get_bit(field, x, y));
        }
        printf("\n");
    }
}


int main(int argc, char** argv)
{
    life_runner game;
    life_runner_init(&game, 32, 18);
    game.neighbors_that_matter = 0b01011010;  // von Neumann neighborhood

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
    }

    life_runner_delete(&game);
    return 0;
}
