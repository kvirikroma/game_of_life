#include <stdio.h>
#include <stdint.h>
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

void life_runner_move_game(life_runner* runner, direction move_direction, uint32_t distance)
{
    bit_array2d* new_field = bit_array2d_init(runner->field->x_size, runner->field->y_size);
    for (uint32_t y = 0; y < runner->field->y_size; y++)
    {
        for (uint32_t x = 0; x < runner->field->x_size; x++)
        {
            if (bit_array2d_get_bit(runner->field, x, y))
            {
                switch (move_direction)
                {
                    case UP:
                    {
                        bit_array2d_set_bit(new_field, x, y - distance, 1);
                        break;
                    }
                    case DOWN:
                    {
                        bit_array2d_set_bit(new_field, x, y + distance, 1);
                        break;
                    }
                    case LEFT:
                    {
                        bit_array2d_set_bit(new_field, x - distance, y, 1);
                        break;
                    }
                    case RIGHT:
                    {
                        bit_array2d_set_bit(new_field, x + distance, y, 1);
                        break;
                    }
                    case UP_LEFT:
                    {
                        bit_array2d_set_bit(new_field, x - distance, y - distance, 1);
                        break;
                    }
                    case UP_RIGHT:
                    {
                        bit_array2d_set_bit(new_field, x + distance, y - distance, 1);
                        break;
                    }
                    case DOWN_LEFT:
                    {
                        bit_array2d_set_bit(new_field, x - distance, y + distance, 1);
                        break;
                    }
                    case DOWN_RIGHT:
                    {
                        bit_array2d_set_bit(new_field, x + distance, y + distance, 1);
                        break;
                    }
                }
            }
        }
    }
    bit_array2d_delete(runner->field);
    runner->field = new_field;
}
