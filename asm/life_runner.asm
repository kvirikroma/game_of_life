%include "bit_array2d_include.asm"
%include "life_runner_include_common.asm"


global life_runner_make_step        ;make one more step
global life_runner_count_neighbors  ;count neighbors of bit
global life_runner_init             ;init life runner with values
global life_runner_delete           ;free the life runner's parts that are allocated in heap


segment .text
    life_runner_init:
        ;param rdi - runner pointer
        ;param rsi - size by X axis
        ;param rdx - size by Y axis
        push rdi
        mov rdi, rsi
        mov rsi, rdx
        call bit_array2d_init
        pop rdi
        mov [rdi + life_runner.field], rax

        ;default rules
        mov byte [rdi + life_runner.neighbors_that_matter], 0FFh
        mov byte [rdi + life_runner.min_neighbors_to_be_born], 3
        mov byte [rdi + life_runner.max_neighbors_to_be_born], 3
        mov byte [rdi + life_runner.min_neighbors_to_exist], 2
        mov byte [rdi + life_runner.max_neighbors_to_exist], 3
        ret

    life_runner_delete:
        ;param rdi - runner pointer
        mov rdi, [rdi + life_runner.field]
        call bit_array2d_delete
        ret
        
    life_runner_count_neighbors:
        ret
        
    life_runner_make_step:
        ret
