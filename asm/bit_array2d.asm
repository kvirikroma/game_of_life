%include "bit_array2d_include_common.asm"

extern malloc
extern free

extern check_pointer_after_malloc

global bit_array2d_init     ; initializes array of needed size in memory
global bit_array2d_delete   ; deletes an bit_array2d* (param rdi - bit_array2d* to delete)
global bit_array2d_get_bit  ; returns value of bit by its coordinates
global bit_array2d_set_bit  ; sets bit value by its coordinates
global bit_array2d_resize   ; change size of bit_array2d (frees old and returns new one)
global bit_array2d_copy_content ; copy bits from one array to another with an offset
global bit_array2d_erase    ; set all bits to zero


segment .text
    bit_field_get_bit:
        ; get value of bit by number
        ; param rdi - address of field
        ; param rsi - number of bit to return
        mov rax, rsi
        shr rax, 3
        mov cl, 7  ; it's also 111b
        and sil, cl
        mov al, [rdi + rax]
        sub cl, sil
        shr al, cl
        and eax, 1
        ret

    bit_field_set_bit:
        ; set value of bit by number
        ; param rdi - address of field
        ; param rsi - number of bit to set
        ; param rdx - value (non-0 values will be accepted as 1)
        mov rax, rsi
        shr rax, 3
        and sil, 111b
        mov r8b, 80h
        mov cl, sil
        shr r8b, cl

        cmp rdx, 0
        je bfsb_zero
            or [rdi + rax], r8b
            jmp bfsb_end
        bfsb_zero:
            not r8b
            and [rdi + rax], r8b
        bfsb_end:

        ret
    
    bit_array2d_erase:
        ; param rdi - bit_array2d*
        mov eax, [rdi + bit_array2d.x_size]
        mov esi, [rdi + bit_array2d.y_size]
        mul esi
        shr rax, 3
        inc rax
        add rdi, BIT_ARRAY2D_SIZE
        mov rcx, rax
        xor eax, eax
        rep stosb
        ret

    bit_array2d_init:
        ; param rdi - size by X axis (in bits)
        ; param rsi - size by Y axis (in bits)
        push rdi
        push rsi

        mov eax, edi
        mul esi
        shr rax, 3
        inc rax

        add rax, BIT_ARRAY2D_SIZE
        push rax
        mov rdi, rax
        call malloc
        mov rdi, rax
        call check_pointer_after_malloc

        pop rcx
        mov rdx, rax
        mov rdi, rax
        xor eax, eax
        rep stosb
        mov rax, rdx
        
        pop rcx
        mov [rax + bit_array2d.y_size], ecx
        pop rcx
        mov [rax + bit_array2d.x_size], ecx

        ret

    bit_array2d_delete:
        ; param rdi - bit_array2d*
        call free
        ret

    bit_array2d_coordinates_to_index:
        ; represent (x, y) coordinates as index in the field
        ; param rdi - bit_array2d*
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; returns index in bit field (it loops on overflow)
        mov r9d, esi
        cmp r9d, 0
        jnl x_nl_than_zero_1
            neg esi
        x_nl_than_zero_1:
        mov r10d, edx
        cmp r10d, 0
        jnl y_nl_than_zero_1
            neg edx
        y_nl_than_zero_1:

        mov ecx, edx
        mov eax, esi
        mov esi, [rdi + bit_array2d.x_size]
        xor edx, edx
        div esi
        mov r8d, edx  ; r8d - X

        mov eax, ecx
        mov ecx, [rdi + bit_array2d.y_size]
        xor edx, edx
        div ecx  ; edx - Y

        cmp r10d, 0
        jnl y_nl_than_zero_2
            xchg edx, ecx
            sub edx, ecx
        y_nl_than_zero_2:
        cmp r9d, 0
        jnl x_nl_than_zero_2
            xchg r8d, esi
            sub r8d, esi
        x_nl_than_zero_2:

        mov eax, edx
        mul dword [rdi + bit_array2d.x_size]
        shl rdx, 32
        or rax, rdx
        add rax, r8
        ret

    bit_array2d_get_bit:
        ; param rdi - bit_array2d*
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; param rcx - disable cycle adressing
        ; returns 0 or 1 (bit value)
        cmp rcx, 0
        je enabled_cycle_addressing
            cmp esi, 0
            jl return_0
            cmp edx, 0
            jl return_0
            cmp esi, [rdi + bit_array2d.x_size]
            jnl return_0
            cmp edx, [rdi + bit_array2d.y_size]
            jnl return_0
        enabled_cycle_addressing:

        push rdi
        call bit_array2d_coordinates_to_index
        pop rdi
        add rdi, bit_array2d.data
        mov rsi, rax
        call bit_field_get_bit

        jmp end_return
        return_0:
            xor eax, eax
        end_return:

        ret

    bit_array2d_set_bit:
        ; param rdi - bit_array2d*
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; param rcx - value (0 or 1), every non-0 value will be accepted as 1
        cmp rcx, 0
        je sb_already_bool
            mov ecx, 1
        sb_already_bool:

        push rdi
        push rcx
        xor ecx, ecx
        call bit_array2d_coordinates_to_index
        pop rdx
        pop rdi
        add rdi, bit_array2d.data
        mov rsi, rax
        call bit_field_set_bit

        ret
    
    bit_array2d_copy_content:
        ; param rdi - bit_array2d* dest
        ; param rsi - bit_array2d* source
        ; param rdx - offset by X axis
        ; param rcx - offset by Y axis
        push rbp
        mov rbp, rsp

        push rdi  ; [rbp-8] - dest
        push rsi  ; [rbp-16] - source
        push rdx  ; [rbp-24] - x offset
        push rcx  ; [rbp-32] - y offset

        mov ecx, [rdi + bit_array2d.y_size]
        walking_through_lines:
            push rcx  ; [rbp-40] - lines left
            mov rdi, [rbp-8]
            mov ecx, [rdi + bit_array2d.x_size]
            walking_through_columns:
                push rcx  ; [rbp-48] - columns left
                
                mov rdi, [rbp-8]
                sub ecx, [rdi + bit_array2d.x_size]
                neg ecx
                mov edx, [rbp-40]
                sub edx, [rdi + bit_array2d.y_size]
                neg edx
                mov rsi, rcx
                sub rsi, [rbp-24]
                sub rdx, [rbp-32]
                mov ecx, 1
                mov rdi, [rbp-16]
                call bit_array2d_get_bit
                mov rdi, [rbp-8]
                mov esi, [rbp-48]
                sub esi, [rdi + bit_array2d.x_size]
                neg esi
                mov edx, [rbp-40]
                sub edx, [rdi + bit_array2d.y_size]
                neg edx
                mov rcx, rax
                call bit_array2d_set_bit

                pop rcx  ; deleted [rbp-48]
                loop walking_through_columns
            pop rcx  ; deleted [rbp-40]
            loop walking_through_lines
        
        leave
        ret
    
    bit_array2d_resize:
        ; returns another bit_array2d* and deletes the old one
        ; param rdi - bit_array2d*
        ; param rsi - size by X axis (in bits)
        ; param rdx - size by Y axis (in bits)
        ; param ecx - offset to move by X axis (not cycled)
        ; param r8d - offset to move by Y axis (not cycled)
        push rbp
        mov rbp, rsp

        push rcx  ; [rbp-8] - x offset
        push r8  ; [rbp-16] - y offset
        
        push rdi  ; [rbp-24] - source
        mov edi, esi
        mov esi, edx
        call bit_array2d_init
        push rax  ; [rbp-32] - destination
        mov rsi, [rbp-24]
        mov rdi, rax
        mov edx, [rbp-8]
        mov ecx, [rbp-16]
        call bit_array2d_copy_content
        mov rdi, [rbp-24]
        call bit_array2d_delete
        pop rax  ; deleted [rbp-32]

        leave
        ret
    
    bit_array2d_is_square_empty:
        ; check if there are some '1' bits in square
        ; no cyclic addressing here!
        ; rdi - bit_array2d*
        ; esi - square X  //must be divisible by 8
        ; edx - square Y
        ; ecx - square size X  //must be divisible by 8; should be EQUAL 64
        ; r8d - square size Y
        mov r9d, ecx
        mov r10d, edx
        shr ecx, 3
        shr esi, 3
        
        ; TODO

        ise_return_0:
            xor eax, eax
            jmp ise_return_end
        ise_return_1:
            mov eax, 1
        ise_return_end:
        ret
