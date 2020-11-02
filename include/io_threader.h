#ifndef GRAPHICS_THREADDER_H
#define GRAPHICS_THREADER_H

#include <pthread.h>

#include "life_drawer.h"


typedef struct
{
    volatile life_drawer drawer;
    volatile bool stop_flag;
    pthread_t input_thread;
    pthread_t output_thread;
    uint32_t window_x;
    uint32_t window_y;
    uint32_t cells_x;
    uint32_t cells_y;
    volatile bool output_drawer_lock;
    volatile bool input_drawer_lock;
    volatile bool* lmb_pressed;
    volatile bool* rmb_pressed;
    volatile bool* move;
}
io_threader;


void io_threader_init(volatile io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, volatile bool* lmb_pressed, volatile bool* rmb_pressed, volatile bool* move);

void io_threader_delete(volatile io_threader* self);

void io_threader_output_lock_drawer(volatile io_threader* self);

void io_threader_output_unlock_drawer(volatile io_threader* self);

void io_threader_input_lock_drawer(volatile io_threader* self);

void io_threader_input_unlock_drawer(volatile io_threader* self);

void* input_thread_function(void* self);

void* output_thread_function(void* self);

#endif
