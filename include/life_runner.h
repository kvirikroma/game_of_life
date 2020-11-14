#ifndef LIFE_RUNNER_H
#define LIFE_RUNNER_H

#include <stdint.h>

#include "bit_array2d.h"


typedef struct
{
    bit_array2d* field;     //ptr to current field
    bit_array2d* field_1;   //1st field
    bit_array2d* field_2;   //2nd field
    uint8_t min_neighbors_to_exist;
    uint8_t max_neighbors_to_exist;
    uint8_t min_neighbors_to_be_born;
    uint8_t max_neighbors_to_be_born;
    uint8_t neighbors_that_matter;
    uint8_t disable_cyclic_adressing;
}
life_runner;

typedef struct
{
    uint32_t size;  // size of uint8_t* data; does not include itself
    uint8_t* data;
}
life_runner_snapshot;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
}
direction;

/*
 * life_runner.neighbors_that_matter shows presence of what neighbors will affect the decision.
 * Each bit of this field is boolean flag of neighbor affecting.
 * They are numbered like this:
 * 0 1 2
 * 3 B 4
 * 5 6 7
 * where B is the bit we are counting neighbors for.
 * For example, 0b01011010 is von Neumann neighborhood.
 */

// Initialize life runner with values
extern void life_runner_init(life_runner* self, uint32_t size_x, uint32_t size_y);

// Free the life runner's parts that are allocated in heap
extern void life_runner_delete(life_runner* self);

// Make one more step
extern void life_runner_make_step(life_runner* self);

// Count neighbors of bit (how many '1' bits are around)
extern uint8_t life_runner_count_neighbors(life_runner* self, int32_t x, int32_t y);

// Move the game in window (distance is in cells)
void life_runner_move_game(life_runner* self, direction move_direction, uint32_t distance);

// Make snapshot of life runner state
life_runner_snapshot life_runner_to_snapshot(life_runner* self);

// Load life runner from snapshot
void life_runner_from_snapshot(life_runner* self, life_runner_snapshot snapshot, bool runner_was_initialized);

// Free memory that contains snapshot data
void life_runner_snapshot_delete(life_runner_snapshot* snapshot);

#endif
