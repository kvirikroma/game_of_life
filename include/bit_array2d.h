#include <stdbool.h>

#include "array2d.h"

typedef struct
{
    uint32_t size_x;
    uint32_t size_y;
    array2d container;
} bit_array2d;


// Initialize bit_array2d of given size in heap
bit_array2d* bit_array2d_init(uint32_t size_x, uint32_t size_y);

// Free memory allocated for bit_array2d
void bit_array2d_delete(bit_array2d* array);

// Returns value of bit by its coordinates
bool bit_array2d_get_bit(uint32_t x, uint32_t y);

// Sets bit value by its coordinates
// Returns 1 on success or 0 on error
bool bit_array2d_set_bit(uint32_t x, uint32_t y, bool value);
