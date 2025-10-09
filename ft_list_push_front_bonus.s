section .text

global ft_list_push_front

extern malloc

; void ft_list_push_front(t_list **begin_list, void *data);
ft_list_push_front:
    push rsp    ; align stack element
    push rdi    ; save begin
    push rsi    ; save data
    mov rdi, 16
    xor rax, rax
    call malloc ; calling malloc(16)
    pop rsi ; restore rsi value from stack
    pop rdi ; restore rdi value from stack
    cmp rax, 0
    jz return ; when malloc failed
    mov rcx, [rdi]
    mov [rax], rcx
    mov [rax + 8], rsi ; new.next = *begin
    mov [rdi], rax

return:
    pop rsp
    ret