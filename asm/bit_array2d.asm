%include "bit_array2d_include_common.asm"

extern malloc
extern free

extern check_pointer_after_malloc

global bit_array2d_init     ; initializes array of needed size in memory
global bit_array2d_delete   ; deletes an bit_array2d* (param rdi - bit_array2d* to delete)
global bit_array2d_get_bit  ; returns value of bit by its coordinates
global bit_array2d_set_bit  ; sets bit value by its coordinates
global bit_array2d_coordinates_to_index ;represent (x, y) coordinates as index in the field
global bit_field_get_bit    ;get value of bit by number
global bit_field_set_bit    ;set value of bit by number


segment .text
    bit_field_get_bit:
        ; param rdi - address of field
        ; param rsi - number of bit to return
        mov rax, rsi
        shr rax, 3
        and esi, 111b
        mov al, [rdi + rax]
        mov ecx, 7
        sub cl, sil
        shr eax, cl
        and al, 1
        ret

    bit_field_set_bit:
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

    bit_array2d_init:
        ; param rdi - size by X axis (in bits)
        ; param rsi - size by Y axis (in bits)
        push rdi
        push rsi

        mov eax, edi
        mul esi
        shr rax, 3
        inc rax

        push rax
        add rax, BIT_ARRAY2D_SIZE
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
        ; param rdi - bit_array2d
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; returns index in bit field (it loops on overflow)
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
        mov eax, edx
        mul dword [rdi + bit_array2d.y_size]
        shl rdx, 32
        or rax, rdx
        add rax, r8
        ret

    bit_array2d_get_bit:
        ; param rdi - bit_array2d
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; returns 0 or 1 (bit value)
        push rdi
        call bit_array2d_coordinates_to_index
        pop rdi
        add rdi, bit_array2d.data
        mov rsi, rax
        call bit_field_get_bit

        ret

    bit_array2d_set_bit:
        ; param rdi - bit_array2d
        ; param rsi - coordinate by X axis (in bits)
        ; param rdx - coordinate by Y axis (in bits)
        ; param rcx - value (0 or 1), every non-0 value will be accepted as 1
        cmp rcx, 0
        je sb_already_bool
            mov ecx, 1
        sb_already_bool:

        push rdi
        push rcx
        call bit_array2d_coordinates_to_index
        pop rdx
        pop rdi
        add rdi, bit_array2d.data
        mov rsi, rax
        call bit_field_set_bit

        ret
