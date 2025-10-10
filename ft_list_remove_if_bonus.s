section .text

global ft_list_remove_if

extern free

; rdi: begin_list (t_list **), rsi: data_ref, rdx: cmp(), rcx: free_fct()
ft_list_remove_if:
    push    rbx                     ; Callee-saved register
    push    r12
    push    r13
    push    r14
    push    r15

    mov     r12, rdi                ; r12 = begin_list
    mov     r13, rsi                ; r13 = data_ref
    mov     r14, rdx                ; r14 = cmp function pointer
    mov     r15, rcx                ; r15 = free_fct function pointer

    cmp     r12, 0
    je      .end_function           ; if begin_lst == NULL
    mov     rbx, [r12]              ; rbx = current_node (*begin_list)
    mov     rax, 0                  ; rax = previous_node, init to NULL

.loop_start:
    cmp     rbx, 0
    je      .end_function           ; if current == NULL 

    mov     rdi, [rbx]              ; arg1 = current->data
    mov     rsi, r13                ; arg2 = data_ref (use saved value)
    call    r14                     ; cmp(current->data, data_ref)

    test    eax, eax                ; if cmp result == 0
    jne     .move_to_next           ; if != 0

    mov     rdi, [rbx]              ; rdi = current->data
    call    r15                     ; free_fct(current->data)

    mov     rdi, rbx                ; rdi = current node
    mov     rdx, [rbx + 8]          ; rdx = current->next (store)
    call    free                    ; free(current)

    cmp     rax, 0                  ; if previous_node == NULL (head node?)
    jne     .link_prev_to_next

    ; head node
    mov     [r12], rdx              ; *begin_list = current->next
    mov     rbx, rdx                ; current = current->next
    jmp     .loop_start

.link_prev_to_next:
    ; middle or last node
    mov     [rax + 8], rdx          ; prev->next = current->next
    mov     rbx, rdx                ; current = current->next
    jmp     .loop_start

.move_to_next:
    mov     rax, rbx                ; prev = current
    mov     rbx, [rbx + 8]          ; current = current->next
    jmp     .loop_start

.end_function:
    pop     r15                     ; Restore Callee-saved register
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret