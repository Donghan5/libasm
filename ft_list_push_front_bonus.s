section .text

global ft_list_push_front

extern malloc
; ------------------------------------------------
;- Register usage:
; 	rdi = t_list **begin_list
; 	rsi = void *data
; 	rax = return value (new node or NULL)
; ------------------------------------------------
ft_list_push_front:
    push    rdi
    push    rsi

    mov     rdi, 16         ; sizeof(t_list)
    call    malloc wrt ..plt ; allocate new node
    test    rax, rax    ; check if malloc succeeded (purpose: NULL check)
    jz      .end
    pop     rsi
    pop     rdi
    mov     [rax], rsi      ; new_node->data = data
    mov     rcx, [rdi]
    mov     [rax + 8], rcx  ; new_node->next = *begin_list
    mov     [rdi], rax      ; *begin_list = new_node
    ret                     ; Sucess, just return
.end:
    pop rsi
    pop rdi ; Clear the buffer in stack (popped)
    ret