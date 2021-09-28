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

// Erase bit_array2d content (except size data)
extern void bit_array2d_erase(bit_array2d* self);

// Free memory allocated for bit_array2d
extern void bit_array2d_delete(bit_array2d* self);

// Returns index in bit field (it loops on overflow)
extern uint32_t bit_array2d_coordinates_to_index(const bit_array2d* self, int32_t x, int32_t y);

// Returns value of bit by its coordinates
extern bool bit_array2d_get_bit(const bit_array2d* self, int32_t x, int32_t y);

// Returns value of bit by its coordinates
extern bool bit_array2d_get_bit_uncycled(const bit_array2d* self, int32_t x, int32_t y);

// Sets bit value by its coordinates
// Returns 1 on success or 0 on error
// Adressing is always cycled
extern void bit_array2d_set_bit(bit_array2d* self, int32_t x, int32_t y, bool value);

// Copy bits from one array to another with an offset
extern void bit_array2d_copy_content(bit_array2d* dest, const bit_array2d* source, uint32_t offset_x, uint32_t offset_y);

// Change size of bit_array2d (frees old and returns new one)
// Copies data to new array with given offset
extern bit_array2d* bit_array2d_resize(bit_array2d* old, uint32_t new_size_x, uint32_t new_size_y, uint32_t offset_x, uint32_t offset_y);

#endif
