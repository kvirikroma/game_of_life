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

typedef enum
{
    TOP_SIDE = 0,
    BOTTOM_SIDE = 1
}
rows_side;

typedef enum
{
    LEFT_SIDE = 0,
    RIGHT_SIDE = 1
}
columns_side;


// Initializes a 2-dimensional array in heap
extern array2d* array2d_init(uint32_t x, uint32_t y, uint16_t item_size);

// Deletes a 2-dimensional array
extern void array2d_delete(array2d* self);

// Returns an item pointer by its coordinates
extern void* array2d_get_item_ptr(const array2d* self, uint32_t x, uint32_t y);

// Copies content from source to dest (with some offset)
void array2d_copy_content(array2d* dest, const array2d* source, int32_t x_offset, int32_t y_offset);

// Add rows to the array2d.
// source_array will be freed
array2d* array2d_add_rows(array2d* source_array, uint32_t count, rows_side operation_side);

// Add columns to the array2d.
// source_array will be freed
array2d* array2d_add_columns(array2d* source_array, uint32_t count, columns_side operation_side);

// Remove rows from the array2d.
// source_array will be freed
array2d* array2d_remove_rows(array2d* source_array, uint32_t count, rows_side operation_side);

// Remove columns from the array2d.
// source_array will be freed
array2d* array2d_remove_columns(array2d* source_array, uint32_t count, columns_side operation_side);

#endif
