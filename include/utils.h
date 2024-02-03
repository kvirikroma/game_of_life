#ifndef UTILS_H
#define UTILS_H

#include "life_runner.h"


// Print error message and exit with code 1
void exit_on_syntax_error(void);

// Return the same pointer or print error message and exit with code 1
void* check_pointer_after_malloc(void* ptr);

// Print bit_array2d
void print_field(bit_array2d* field);

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

uint64_t min(uint64_t num1, uint64_t num2);

uint64_t max(uint64_t num1, uint64_t num2);

#endif
