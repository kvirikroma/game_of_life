#ifndef GRAPHICS_THREADDER_H
#define GRAPHICS_THREADER_H

#include <pthread.h>

#include "life_drawer.h"


typedef struct
{
    life_drawer drawer;
    bool stop_flag;
    pthread_t input_thread;
    pthread_t output_thread;
    uint32_t window_x;
    uint32_t window_y;
    uint32_t cells_x;
    uint32_t cells_y;
    volatile bool output_pause;
    volatile bool output_pause_response;
    volatile bool input_pause;
    volatile bool input_pause_response;
    bool* lmb_pressed;
    bool* rmb_pressed;
    bool* move;
}
io_threader;


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, bool* lmb_pressed, bool* rmb_pressed, bool* move);

void io_threader_delete(io_threader* self);

void io_threader_input_pause(io_threader* self);

void io_threader_input_unpause(io_threader* self);

void io_threader_output_pause(io_threader* self);

void io_threader_output_unpause(io_threader* self);

void* input_thread_function(void* self);

void* output_thread_function(void* self);

#endif
