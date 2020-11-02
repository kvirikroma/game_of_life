#ifndef UTILS_H
#define UTILS_H

#include "bit_array2d.h"
#include "array2d.h"


// Print error message and exit with code 1
void exit_on_syntax_error(void);

// Return the same pointer or print error message and exit with code 1
void* check_pointer_after_malloc(void* ptr);

// Print bit_array2d
void print_field(bit_array2d* field);

// Print array2d of uint32_t items
void print_array2d(array2d* array);

// Sleep given amount of milliseconds
void sleep_ms(double milliseconds);

#endif
