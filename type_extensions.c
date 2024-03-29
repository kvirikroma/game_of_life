#include <string.h>
#include <stdlib.h>

#include "include/life_runner.h"


void life_runner_move_game(life_runner* self, direction move_direction, uint32_t distance)
{
    bit_array2d* new_field;
    if (self->field == self->field_1)
    {
        new_field = self->field_2;
    }
    else
    {
        new_field = self->field_1;
    }
    bit_array2d_erase(new_field);
    for (uint32_t y = 0; y < self->field->y_size; y++)
    {
        for (uint32_t x = 0; x < self->field->x_size; x++)
        {
            if (bit_array2d_get_bit((bit_array2d*)self->field, x, y))
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
    self->field = new_field;
}


void life_runner_move_game_by_coordinates(life_runner* self, int32_t x_offset, int32_t y_offset)
{
    bit_array2d* new_field;
    if (self->field == self->field_1)
    {
        new_field = self->field_2;
    }
    else
    {
        new_field = self->field_1;
    }
    bit_array2d_erase(new_field);
    for (uint32_t y = 0; y < self->field->y_size; y++)
    {
        for (uint32_t x = 0; x < self->field->x_size; x++)
        {
            if (bit_array2d_get_bit((bit_array2d*)self->field, x, y))
            {
                bit_array2d_set_bit(new_field, x + x_offset, y + y_offset, 1);
            }
        }
    }
    self->field = new_field;
}


life_runner_snapshot life_runner_to_snapshot(life_runner* self)
{
    life_runner_snapshot result;
    result.size = 6 +  // life_runner bytes that define config
        ((self->field->x_size * self->field->y_size) >> 3) + 1 +  // size of bit part of bit_array2d
        sizeof(bit_array2d);  // size of bit_array2d service part
    result.data = (uint8_t*)malloc(result.size);
    result.data[0] = self->max_neighbors_to_be_born;
    result.data[1] = self->min_neighbors_to_be_born;
    result.data[2] = self->max_neighbors_to_exist;
    result.data[3] = self->min_neighbors_to_exist;
    result.data[4] = self->neighbors_that_matter;
    result.data[5] = self->disable_cyclic_adressing;
    memcpy(result.data + 6, self->field, result.size - 6);
    return result;
}


void life_runner_from_snapshot(life_runner* self, life_runner_snapshot snapshot, bool runner_was_initialized)
{
    self->max_neighbors_to_be_born = snapshot.data[0];
    self->min_neighbors_to_be_born = snapshot.data[1];
    self->max_neighbors_to_exist = snapshot.data[2];
    self->min_neighbors_to_exist = snapshot.data[3];
    self->neighbors_that_matter = snapshot.data[4];
    self->disable_cyclic_adressing = snapshot.data[5];
    if (runner_was_initialized)
    {
        bit_array2d_delete(self->field_1);
        bit_array2d_delete(self->field_2);
    }
    bit_array2d* array_in_data = (bit_array2d*)(snapshot.data + 6);
    self->field = self->field_1 = bit_array2d_init(array_in_data->x_size, array_in_data->y_size);
    self->field_2 = bit_array2d_init(array_in_data->x_size, array_in_data->y_size);
    memcpy(self->field, array_in_data, snapshot.size - 6);
}


void life_runner_snapshot_delete(life_runner_snapshot* snapshot)
{
    free(snapshot->data);
    *snapshot = (life_runner_snapshot){0, 0};
}
