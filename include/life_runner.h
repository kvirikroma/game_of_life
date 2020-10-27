#include <stdint.h>
#include "bit_array2d.h"

typedef struct
{
    uint8_t min_neighbors_to_exist;
    uint8_t max_neighbors_to_exist;
    uint8_t min_neighbors_to_be_born;
    uint8_t max_neighbors_to_be_born;
    uint8_t neighbors_that_matter;
    bit_array2d* field;
}
life_runner;

/*
 * life_runner.neighbors_that_matter shows presence of what neighbors will affect the decision.
 * Each bit of this field is boolean flag of neighbor affecting.
 * They are numering like this:
 * 0 1 2
 * 3 B 4
 * 5 6 7
 * where B is the bit we are counting neighbors for
 */

// Make one more step
extern void life_runner_make_step(life_runner* runner);

// Count neighbors of bit (how many '1' bits are around)
extern uint8_t life_runner_count_neighbors(life_runner* runner, uint32_t x, uint32_t y);

// Initialize life runner with values
extern void life_runner_init(life_runner* runner, uint32_t size_x, uint32_t size_y, uint8_t neighbors_that_matter);

// Free the life runner's parts that are allocated in heap
extern void life_runner_delete(life_runner* runner);
