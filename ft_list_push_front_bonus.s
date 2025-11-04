section .text

global ft_list_push_front

extern malloc

ft_list_push_front: ; rdi: t_list **begin_list
                     ; rsi: void *data
                     ; returns: void
    push    rdi
    push    rsi

    mov     rdi, 16         ; sizeof(t_list)
    call    malloc wrt ..plt
    test    rax, rax
    jz      .end

    pop     rsi             ; data
    pop     rdi             ; begin_list

    mov     [rax], rsi      ; new_node->data = data
    mov     rcx, [rdi]
    mov     [rax + 8], rcx  ; new_node->next = *begin_list
    mov     [rdi], rax      ; *begin_list = new_node

.end:
    ret