#ifndef IO_THREADER_H
#define IO_THREADER_H

#include <pthread.h>

#include "life_drawer.h"
#include "event_listener.h"


typedef struct
{
    life_drawer drawer;
    bool stop_flag;
    pthread_t input_thread;
    pthread_t output_thread;
    event_listener input;
    pthread_mutex_t drawer_lock;
    bool threads_started;
    volatile bool redrawed;
    bool mouse_inited;
}
io_threader;

typedef struct
{
    coordinates window_size;
    coordinates game_field_size;
    io_threader* threader;
}
output_thread_params;

typedef struct
{
    io_threader* threader;
}
input_thread_params;



// Initialize the I/O multi-threader
void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y);

// Kill all threads, free all allocated memory and delete the I/O multi-threader
void io_threader_delete(io_threader* self);

// Lock life_drawer access
void io_threader_lock_drawer(io_threader* self);

// Unlock life_drawer access
void io_threader_unlock_drawer(io_threader* self);

// Function to be run in the input thread
void* input_thread_function(void* self);

// Function to be run in the output thread
void* output_thread_function(void* self);

#endif
