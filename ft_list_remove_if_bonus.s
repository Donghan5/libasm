section .text

global ft_list_remove_if

extern free

; rdi: begin_list (t_list **), rsi: data_ref, rdx: cmp(), rcx: free_fct()
ft_list_remove_if:
    ; --- Prologue: Save callee-saved registers and align stack ---
    push    rbp                     ; Save Base Pointer
    push    rbx                     ; Save general purpose registers we will use
    push    r12
    push    r13
    push    r14
    push    r15
    sub     rsp, 8                  ; Align stack to a 16-byte boundary

    ; --- Setup variables ---
    mov     r12, rdi                ; r12 = begin_list
    mov     r13, rsi                ; r13 = data_ref
    mov     r14, rdx                ; r14 = cmp function pointer
    mov     r15, rcx                ; r15 = free_fct function pointer

    ; --- Handle empty list case ---
    cmp     r12, 0
    je      .end_function           ; if begin_list == NULL, exit
    mov     rbx, [r12]              ; rbx = current_node (*begin_list)
    mov     rbp, 0                  ; rbp = previous_node = NULL

.loop_start:
    cmp     rbx, 0
    je      .end_function           ; if current_node == NULL, we are done

    ; --- Call cmp(current->data, data_ref) ---
    mov     rdi, [rbx]              ; 1st arg for cmp: current_node->data
    mov     rsi, r13                ; 2nd arg for cmp: data_ref
    call    r14                     ; call cmp()

    test    eax, eax                ; Check if result is 0
    jne     .move_to_next           ; If not 0, skip removal and move to next node

    ; --- Node Removal Logic ---
    mov     rdi, [rbx]              ; 1st arg for free_fct: current_node->data
    call    r15                     ; call free_fct()

    mov     rdx, [rbx + 8]          ; Save next pointer before freeing: rdx = current_node->next
    push    rdx
    mov     rdi, rbx                ; 1st arg for free: current_node
    call    free wrt ..plt          ; call free()

    pop     rdx

    cmp     rbp, 0                  ; Was this the head of the list (previous_node == NULL)?
    jne     .link_prev_to_next

    ; Case 1: Removing the head node
    mov     [r12], rdx              ; *begin_list = saved next_pointer
    mov     rbx, rdx                ; current_node = saved next_pointer
    jmp     .loop_start

.link_prev_to_next:
    ; Case 2: Removing a middle/tail node
    mov     [rbp + 8], rdx          ; previous_node->next = saved next_pointer
    mov     rbx, rdx                ; current_node = saved next_pointer
    jmp     .loop_start

.move_to_next:
    mov     rbp, rbx                ; previous_node = current_node
    mov     rbx, [rbx + 8]          ; current_node = current_node->next
    jmp     .loop_start

.end_function:
    ; --- Epilogue: Restore stack and registers ---
    add     rsp, 8                  ; De-align stack, restoring original offset
    pop     r15                     ; Restore registers in reverse order
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret
