#ifndef BIT_ARRAY2D_H
#define BIT_ARRAY2D_H

#include <stdbool.h>
#include <stdint.h>


typedef struct
{
    uint32_t x_size;
    uint32_t y_size;
}
bit_array2d;


// Initialize bit_array2d of given size in heap
extern bit_array2d* bit_array2d_init(uint32_t size_x, uint32_t size_y);

// Free memory allocated for bit_array2d
extern void bit_array2d_delete(bit_array2d* array);

// Returns index in bit field (it loops on overflow)
extern uint32_t bit_array2d_coordinates_to_index(bit_array2d* array, uint32_t x, uint32_t y);

// Returns value of bit by its coordinates
extern bool bit_array2d_get_bit(bit_array2d* array, uint32_t x, uint32_t y);

// Sets bit value by its coordinates
// Returns 1 on success or 0 on error
extern void bit_array2d_set_bit(bit_array2d* array, uint32_t x, uint32_t y, bool value);

#endif
