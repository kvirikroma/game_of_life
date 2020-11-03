#ifndef IO_THREADER_H
#define IO_THREADER_H

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
    pthread_mutex_t drawer_lock;
    coordinates last_mouse_position;
    bool* lmb_pressed;
    bool* rmb_pressed;
    bool* move;
    uint8_t* speed;
    bool draw_line;
}
io_threader;


void io_threader_init(io_threader* self, uint32_t window_x, uint32_t window_y, uint32_t cells_x, uint32_t cells_y, bool* lmb_pressed, bool* rmb_pressed, bool* move, uint8_t* speed);

void io_threader_delete(io_threader* self);

void io_threader_lock_drawer(io_threader* self);

void io_threader_unlock_drawer(io_threader* self);

void* input_thread_function(void* self);

void* output_thread_function(void* self);

#endif
