#include <stdbool.h>

#include "array2d.h"

typedef struct
{
    uint32_t size_x;
    uint32_t size_y;
    array2d container;
} bit_array2d;


bit_array2d* bit_array2d_init(uint32_t size_x, uint32_t size_y);

void bit_array2d_delete(bit_array2d* array);

bool bit_array2d_get_bit(uint32_t x, uint32_t y);

bool bit_array2d_set_bit(uint32_t x, uint32_t y, bool value);
