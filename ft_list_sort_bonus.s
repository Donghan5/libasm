section .text

global ft_list_sort

; void ft_list_sort(t_list **begin_list, int (*cmp)());
ft_list_sort:
    push rbx    ; current element
    push r12    ; original address of list (first element)
    cmp rdi, 0  ; if begin_list == NULL (null check)
    jz restore  ; shut down program (if begin_list is NULL)
    mov r12, [rdi] ; r12 = (*begin_list)
    cmp rsi, 0 ; if ((*cmp)() == NULL)
    jz return
    jmp compare_main

; ------ outer loop ------
increment_main:
    mov rcx, [rdi] ; rcx = (*begin)
    mov rbx, [rcx] ; rbx = rcx->next
    mov [rdi], rbx  ; *begin = rbx

compare_main:
    cmp QWORD [rdi], 0  ; checking *begin == NULL (arrive at last element)
    jz return
    mov rcx, [rdi]
    mov rbx, [rcx]

; ------ inner loop ------
compare_single:
    cmp rbx, 0  ; current == NULL
    jz increment_main ; compare action is complete, move to outer loop

compare:
    push rdi
    push rsi
    mov rax, rsi    ; register the address of cmp function
    mov rcx, [rdi]  ; rcx = *begin
    mov rdi, [rcx + 8]  ; rdi = rcx->data (+8 to take a data)
    mov rsi, [rbx + 8]  ; rsi = rbx->data
    call rax        ; cmp(rdi, rsi)
    pop rsi
    pop rdi
    cmp rax, 0      ; checking return value of cmp > 0
    jg swap

increment_single:
    mov rcx, [rbx]
    mov rbx, rcx    ; current = current->next
    jmp compare_single

swap:
    mov r8, [rdi]   ; r8 = (*begin)
    mov rcx, [r8 + 8]   ; rcx = r8->data
    mov rax, [rbx + 8]  ; rax = rbx->data (current node)
    mov [r8 + 8], rax   
    mov [rbx + 8], rcx
    jmp increment_single

return:
    mov [rdi], r12  ; *begin = first (init begin pointer)

restore:
    pop r12
    pop rbx
    ret