#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdint.h>


typedef struct
{
    uint32_t x_size;
    uint32_t y_size;
    uint16_t item_size;
}
array2d;


// Initializes a 2-dimensional array in heap
extern array2d* array2d_init(uint32_t x, uint32_t y, uint16_t item_size);

// Deletes a 2-dimensional array
extern void array2d_delete(array2d* array);

// Returns an item pointer by its coordinates
extern void* array2d_get_item_ptr(array2d* array, uint32_t x, uint32_t y);

#endif
