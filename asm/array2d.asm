%include "include/array_2d_include_common.asm"

extern malloc
extern free

extern check_pointer_after_malloc

global array2d_init         ; initializes array of needed size in memory
global array2d_delete       ; deletes an array2d* (param rdi - array2d* to delete)
global array2d_get_item_ptr ; returns pointer to an item with given coordinates


segment .text
    array2d_delete equ free

    array2d_init:
        ; initializes an 2-dimensional array in memory
        ; param rdi - size by X axis (maximum is 4.294.967.295)
        ; param rsi - size by Y axis (maximum is 4.294.967.295)
        ; param rdx - size of single item (maximum is 65.535)
        ; returns array2d* allocated in the heap
        push rdi
        push rsi
        push rdx

        mov eax, edi
        mov ecx, edx
        mul esi
        mul rcx
        push rax
        add rax, ARRAY2D_SIZE
        call malloc
        mov rdi, rax
        call check_pointer_after_malloc

        pop rcx
        mov rdx, rax
        mov rdi, rax
        xor eax, eax
        rep stosb
        
        pop rcx
        mov [rdx + array2d.item_size], cx
        pop rcx
        mov [rdx + array2d.y_size], ecx
        pop rcx
        pop [rdx + array2d.x_size], ecx

        ret
    
    array2d_get_item_ptr:
        ; param rdi - array2d* to get item from it
        ; param rsi - X-coordinate of an item
        ; param rdx - Y-coordinate of an item
        ; returns item*
        cmp esi, edx  ; zero their upper parts

        movzx r8, dword [rdi + array2d.x_size]
        cmp rsi, r8
        jnl gip_return_nullptr
        movzx r8, dword [rdi + array2d.y_size]
        cmp rdx, r8
        jnl gip_return_nullptr

        mov eax, esi
        movzx r8, word [rdi + array2d.item_size]  ; r8 - item size
        mul r8d
        mov r9, rax  ; r9 - x_coord * item_size
        mov eax, [rdi + array2d.x_size]
        mul r8d
        mul edx
        add r9

        jmp gip_exit_normally
        gip_return_nullptr:
            xor eax, eax
        gip_exit_normally:

        ret
    
    array2d_add_row:
        ret
    
    array2d_add_column:
        ret
