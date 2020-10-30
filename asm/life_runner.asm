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
        mov rsi, [rdi + life_runner.field]
        mov qword [rdi + life_runner.field], 0
        mov rdi, rsi
        call bit_array2d_delete
        ret
        
    life_runner_count_neighbors:
        ;param rdi - runner pointer
        ;param rsi (esi actually) - X coordinate
        ;param rdx (edx actually) - Y coordinate
        push rbp
        mov rbp, rsp

        push rdi  ; [rbp-8] - runner
        push rsi  ; [rbp-16] - X
        push rdx  ; [rbp-24] - Y

        %macro reload_variables 0
            mov rdi, [rbp-8]
            mov rsi, [rbp-16]
            mov rdx, [rbp-24]
        %endmacro

        %macro save_bit 1
            mov rdi, rcx
            call bit_array2d_get_bit
            shl al, %1
            or [rbp-32], al
        %endmacro

        ;-1, -1
        mov rcx, [rdi + life_runner.field]
        dec esi
        dec edx
        mov rdi, rcx
        call bit_array2d_get_bit
        push rax  ; [rbp-32] - result

        ;0, -1
        reload_variables
        mov rcx, [rdi + life_runner.field]
        dec edx
        save_bit 1

        ;+1, -1
        reload_variables
        mov rcx, [rdi + life_runner.field]
        inc esi
        dec edx
        save_bit 2

        ;-1, 0
        reload_variables
        mov rcx, [rdi + life_runner.field]
        dec esi
        save_bit 3

        ;+1, 0
        reload_variables
        mov rcx, [rdi + life_runner.field]
        inc esi
        save_bit 4

        ;-1, +1
        reload_variables
        mov rcx, [rdi + life_runner.field]
        dec esi
        inc edx
        save_bit 5

        ;0, +1
        reload_variables
        mov rcx, [rdi + life_runner.field]
        inc edx
        save_bit 6

        ;+1, +1
        reload_variables
        mov rcx, [rdi + life_runner.field]
        inc esi
        inc edx
        save_bit 7


        ;get number of neighbors from mask
        mov rdi, [rbp-8]
        mov rax, [rbp-32]
        and al, [rdi + life_runner.neighbors_that_matter]
        and eax, 0FFh
        mov [rbp-32], rax

        ; checking for popcnt instruction
        cpuid
        mov edx, [rbp-32]
        shr ecx, 23
        
        and cl, 1
        jz not_have_sse4
            popcnt eax, edx
            jmp lrcn_end
        not_have_sse4:
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

        leave
        ret
        
    life_runner_make_step:
        ;param rdi - runner pointer
        push rbp
        mov rbp, rsp

        push rdi  ; [rbp-8] - runner pointer
        mov rdi, [rdi + life_runner.field]
        push rdi  ; [rbp-16] - old field
        mov esi, [rdi + bit_array2d.y_size]
        push rsi  ; [rbp-24] - Y size
        mov edi, [rdi + bit_array2d.x_size]
        push rdi  ; [rbp-32] - X size
        call bit_array2d_init
        push rax  ; [rbp-40] - new field

        %define runner    [rbp-8]
        %define old_field [rbp-16]
        %define y_size    [rbp-24]
        %define x_size    [rbp-32]
        %define new_field [rbp-40]
        
        mov rcx, y_size
        walking_through_lines:
            push rcx  ; [rbp-48] - (length) minus (current line (Y coordinate))
            mov rcx, x_size
            walking_through_columns:
                push rcx  ; [rbp-56] - (length) minus (current column (X coordinate))
                
                mov rdi, [rbp-8]
                mov rsi, x_size
                sub rsi, rcx
                mov rdx, y_size
                sub rdx, [rbp-48]
                push rsi  ; [rbp-64] - X coordinate
                push rdx  ; [rbp-72] - Y coordinate
                call life_runner_count_neighbors
                mov rdx, [rbp-72]
                mov rsi, [rbp-64]
                push rax  ; [rbp-80] - neighbors
                mov rdi, old_field
                call bit_array2d_get_bit
                
                mov rdx, [rbp-80]
                cmp eax, 0
                je empty_cell
                    mov rdi, runner
                    cmp dl, [rdi + life_runner.min_neighbors_to_exist]
                    jl unset_cell
                    cmp dl, [rdi + life_runner.max_neighbors_to_exist]
                    jg unset_cell
                    jmp set_cell
                empty_cell:
                    mov rdi, runner
                    cmp dl, [rdi + life_runner.min_neighbors_to_be_born]
                    jl unset_cell
                    cmp dl, [rdi + life_runner.max_neighbors_to_be_born]
                    jg unset_cell
                    jmp set_cell

                set_cell:
                    mov rdi, new_field
                    mov rsi, [rbp-64]
                    mov rdx, [rbp-72]
                    mov cl, 1
                    call bit_array2d_set_bit
                    jmp end_setting_cell
                unset_cell:
                    mov rdi, new_field
                    mov rsi, [rbp-64]
                    mov rdx, [rbp-72]
                    mov ecx, 0
                    call bit_array2d_set_bit
                end_setting_cell:

                add rsp, 24  ; 8*3

                pop rcx  ; deleted [rbp-56]
                dec rcx
                jnz walking_through_columns  ; loop far
            pop rcx  ; deleted [rbp-48]
            dec rcx
            jnz walking_through_lines  ; loop far
        
        mov rdi, runner
        mov rsi, new_field
        mov [rdi + life_runner.field], rsi
        mov rdi, old_field
        call bit_array2d_delete
        
        %undef runner
        %undef old_field
        %undef y_size
        %undef x_size
        %undef new_field

        leave
        ret
