%include "bit_array2d_include.asm"
%include "life_runner_include_common.asm"


global life_runner_make_step        ;make one more step
global life_runner_count_neighbors  ;count neighbors of bit
global life_runner_init             ;init life runner with values
global life_runner_delete           ;free the life runner's parts that are allocated in heap


segment .text
    life_runner_init:
        ret

    life_runner_delete:
        ret
        
    life_runner_count_neighbors:
        ret
        
    life_runner_make_step:
        ret
