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

extern void life_runner_make_step();
extern uint8_t life_runner_count_neighbors();
extern void life_runner_init(life_runner*);
extern void life_runner_delete(life_runner);
