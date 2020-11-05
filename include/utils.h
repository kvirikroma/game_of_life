#ifndef UTILS_H
#define UTILS_H

#include "array2d.h"
#include "life_runner.h"


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

// Save life_runner_snapshot to file
void save_runner_snapshot_to_file(life_runner_snapshot* snapshot, const char* filename);

// Read life_runner_snapshot to file
// Return true on success
// snapshot is initialized on success, else - not
bool load_runner_snapshot_from_file(life_runner_snapshot* snapshot, const char* filename, bool snapshot_was_initialized);

// Get milisecond of current day
int64_t get_current_millisecond(void);

#endif
