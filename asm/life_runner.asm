%include "bit_array2d_include.asm"
%include "life_runner_include_common.asm"

extern change_cell_state

global life_runner_make_step        ;make one more step
global life_runner_count_neighbors  ;count neighbors of bit
global life_runner_init             ;init life runner with values
global life_runner_delete           ;free the life runner's parts that are allocated in heap


segment .data
    popcnt_executor dq 1


segment .text
    life_runner_init:
        ;param rdi - runner pointer
        ;param rsi - size by X axis
        ;param rdx - size by Y axis
        push rbp
        mov rbp, rsp

        push rdi  ; [rbp-8] - runner
        push rsi  ; [rbp-16] - X
        push rdx  ; [rbp-24] - Y

        mov rdi, rsi
        mov rsi, rdx
        call bit_array2d_init
        mov rdi, [rbp-8]
        mov [rdi + life_runner.field], rax
        mov [rdi + life_runner.field_1], rax
        mov rdi, [rbp-16]
        mov rsi, [rbp-24]
        call bit_array2d_init
        mov rdi, [rbp-8]
        mov [rdi + life_runner.field_2], rax

        ;default rules
        mov byte [rdi + life_runner.neighbors_that_matter], 0FFh
        mov byte [rdi + life_runner.min_neighbors_to_be_born], 3
        mov byte [rdi + life_runner.max_neighbors_to_be_born], 3
        mov byte [rdi + life_runner.min_neighbors_to_exist], 2
        mov byte [rdi + life_runner.max_neighbors_to_exist], 3
        mov byte [rdi + life_runner.disable_cyclic_adressing], 0

        ; checking for popcnt instruction
        push rbx
        mov eax, 1
        cpuid
        pop rbx
        shr rcx, 23
        and cl, 1
        mov rax, have_popcnt
        mov rdx, not_have_popcnt
        cmp cl, 0
        cmove rax, rdx
        mov [popcnt_executor], rax

        leave
        ret

    life_runner_delete:
        ;param rdi - runner pointer
        push rdi
        mov rsi, [rdi + life_runner.field_1]
        mov qword [rdi + life_runner.field_1], 0
        mov rdi, rsi
        call bit_array2d_delete
        mov rdi, [rsp]
        mov rsi, [rdi + life_runner.field_2]
        mov qword [rdi + life_runner.field_2], 0
        mov rdi, rsi
        call bit_array2d_delete
        pop rdi
        mov qword [rdi + life_runner.field], 0
        ret
        
    life_runner_count_neighbors:
        ;param rdi - runner pointer
        ;param esi - X coordinate
        ;param edx - Y coordinate
        
        push r15
        push r14
        push r13
        push r12
        push rbx
        push rdi  ; [rsp] - runner
        mov r14, [rdi + life_runner.field]  ; r14 - field
        mov r12, rsi  ; r12 - X
        mov r13, rdx  ; r13 - Y 
        mov rax, bit_array2d_get_bit
        mov r15, bit_array2d_get_bit_uncycled
        cmp byte [rdi + life_runner.disable_cyclic_adressing], 0
        cmove r15, rax  ; r15 - bit getter

        %macro reload_variables 0
            mov rsi, r12
            mov rdx, r13
        %endmacro

        %macro save_bit 1
            mov rdi, r14
            call r15
            shl al, %1
            or bl, al
        %endmacro

        ;-1, -1
        dec esi
        dec edx
        mov cl, bh
        and ecx, 1
        mov rdi, r14
        call r15
        mov bl, al  ; bl - result

        ;0, -1
        reload_variables
        dec edx
        save_bit 1

        ;+1, -1
        reload_variables
        inc esi
        dec edx
        save_bit 2

        ;-1, 0
        reload_variables
        dec esi
        save_bit 3

        ;+1, 0
        reload_variables
        inc esi
        save_bit 4

        ;-1, +1
        reload_variables
        dec esi
        inc edx
        save_bit 5

        ;0, +1
        reload_variables
        inc edx
        save_bit 6

        ;+1, +1
        reload_variables
        inc esi
        inc edx
        save_bit 7


        ;get number of neighbors from mask
        pop r15
        movzx edx, bl
        and dl, [r15 + life_runner.neighbors_that_matter]
        
        jmp [popcnt_executor]
        have_popcnt:
            popcnt eax, edx
            jmp lrcn_end
        not_have_popcnt:
            mov ecx, 8
            xor eax, eax
            lrcn_counting:
                mov r8d, edx
                dec ecx
                shr r8d, cl
                inc ecx
                and r8d, 1
                add eax, r8d
                loop lrcn_counting
        lrcn_end:

        pop rbx
        pop r12
        pop r13
        pop r14
        pop r15
        ret
        
    life_runner_make_step:
        ;param rdi - runner pointer
        push rbp
        mov rbp, rsp

        mov rcx, [rdi + life_runner.field]
        
        mov rdx, [rdi + life_runner.field_1]  ; tmp
        mov rax, [rdi + life_runner.field_2]
        cmp rcx, rdx
        cmovne rax, rdx

        push rcx  ; [rbp-8] - old field
        mov esi, [rcx + bit_array2d.y_size]
        push rsi  ; [rbp-16] - Y size
        mov ecx, [rcx + bit_array2d.x_size]
        push rcx  ; [rbp-24] - X size
        push rax  ; [rbp-32] - new field

        push r15
        push r14
        push r13
        push r12
        push rbx
        mov rbx, rdi  ; rbx - runner pointer

        %define runner    rbx
        %define old_field [rbp-8]
        %define y_size    [rbp-16]
        %define x_size    [rbp-24]
        %define new_field [rbp-32]
        
        mov r12, y_size  ; r12 = ((length) - (current line (Y coordinate)))
        walking_through_lines:
            mov r13, x_size  ; r13 = ((length) - (current column (X coordinate)))
            walking_through_columns:
                mov rdi, runner
                mov rsi, x_size
                sub rsi, r13
                mov rdx, y_size
                sub rdx, r12
                mov r14, rsi  ; r14 - X coordinate
                mov r15, rdx  ; r15 - Y coordinate
                call life_runner_count_neighbors
                mov rsi, rax
                mov rdi, runner
                mov rdx, r14
                mov rcx, r15
                mov r8, new_field
                call change_cell_state

                dec r13
                jnz walking_through_columns  ; loop far
            dec r12
            jnz walking_through_lines  ; loop far
        
        mov rdi, runner
        pop rbx
        pop r12
        pop r13
        pop r14
        pop r15
        
        mov rsi, new_field
        mov [rdi + life_runner.field], rsi
        
        %undef runner
        %undef old_field
        %undef y_size
        %undef x_size
        %undef new_field

        leave
        ret
