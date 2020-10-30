#include <string.h>

#include "include/life_runner.h"
#include "include/array2d.h"


void life_runner_move_game(life_runner* self, direction move_direction, uint32_t distance)
{
    bit_array2d* new_field = bit_array2d_init(self->field->x_size, self->field->y_size);
    for (uint32_t y = 0; y < self->field->y_size; y++)
    {
        for (uint32_t x = 0; x < self->field->x_size; x++)
        {
            if (bit_array2d_get_bit(self->field, x, y))
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
    bit_array2d_delete(self->field);
    self->field = new_field;
}


void array2d_copy_content(array2d* dest, const array2d* source, int32_t x_offset, int32_t y_offset)
{
    for (uint32_t y = 0; y < source->y_size; y++)
    {
        for (uint32_t x = 0; x < source->x_size; x++)
        {
            void* source_item = array2d_get_item_ptr(source, x, y);
            void* dest_item = array2d_get_item_ptr(dest, x + x_offset, y + y_offset);
            if (dest_item && source_item)
            {
                memcpy(dest_item, source_item, source->item_size);
            }
        }
    }
}


array2d* array2d_add_rows(array2d* source_array, uint32_t count, rows_side operation_side)
{
    array2d* new_array = array2d_init(source_array->x_size, source_array->y_size + count, source_array->item_size);
    int32_t offset = 0;
    if (operation_side == TOP_SIDE)
    {
        offset = count;
    }
    array2d_copy_content(new_array, source_array, 0, offset);
    array2d_delete(source_array);
    return new_array;
}


array2d* array2d_add_columns(array2d* source_array, uint32_t count, columns_side operation_side)
{
    array2d* new_array = array2d_init(source_array->x_size + count, source_array->y_size, source_array->item_size);
    int32_t offset = 0;
    if (operation_side == LEFT_SIDE)
    {
        offset = count;
    }
    array2d_copy_content(new_array, source_array, offset, 0);
    array2d_delete(source_array);
    return new_array;
}


array2d* array2d_remove_rows(array2d* source_array, uint32_t count, rows_side operation_side)
{
    array2d* new_array = array2d_init(source_array->x_size, source_array->y_size - count, source_array->item_size);
    int32_t offset = 0;
    if (operation_side == TOP_SIDE)
    {
        offset = -count;
    }
    array2d_copy_content(new_array, source_array, 0, offset);
    array2d_delete(source_array);
    return new_array;
}


array2d* array2d_remove_columns(array2d* source_array, uint32_t count, columns_side operation_side)
{
    array2d* new_array = array2d_init(source_array->x_size - count, source_array->y_size, source_array->item_size);
    int32_t offset = 0;
    if (operation_side == LEFT_SIDE)
    {
        offset = -count;
    }
    array2d_copy_content(new_array, source_array, offset, 0);
    array2d_delete(source_array);
    return new_array;
}
