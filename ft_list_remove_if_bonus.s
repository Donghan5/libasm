section .text

global ft_list_remove_if

extern free

; void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));
; rdi: begin_list, rsi: data_ref, rdx: cmp, rcx: free_fct
ft_list_remove_if:
    ; --- Function Prologue: Save callee-saved registers that will be used ---
    push rbx                ; Save rbx, will be used for the 'prev' pointer.
    push r12                ; Save r12, will be used to temporarily store 'current->next'.
    push r13                ; call-saved register
    sub rsp, 8              ; Align stack to 16 byte boundary

    ; --- Initialization ---
    mov r13, rdi
    cmp  rdi, 0             ; Check if begin_list is NULL.
    jz   end_function       ; If so, there's nothing to do.
    mov  rax, [rdi]         ; rax = *begin_list. Use rax as the 'current' pointer.
    xor  rbx, rbx           ; rbx = 0. Initialize the 'prev' pointer to NULL.

loop_start:
    cmp  rax, 0             ; Check if current == NULL.
    jz   end_function       ; If it is, we've reached the end of the list.

    ; --- Prepare for the 'cmp' function call ---
    ; Save registers that might be modified by the function call (caller-saved).
    push rax
    push rbx
    push rdi
    push rsi
    push rdx                ; cmp
    push rcx                ; free_fct

    mov  r8, rdx            ; ← cmp 함수 포인터를 r8에 백업
    mov  rdi, [rax + 8]
    call r8                 ; ← rdx 대신 r8 호출!

    pop  rcx
    pop  rdxmake 
    pop  rsi
    pop  rdi
    pop  rbx
    pop  rax

    ; --- Check the result from the cmp function ---
    cmp  eax, 0             ; Is the return value (in eax) equal to 0?
    jne  move_to_next       ; If not equal (Jump if Not Equal), skip deletion and move to the next node.

delete_node:
    mov  r12, [rax]         ; r12 = current->next. Save the next node's address before freeing the current one.

    ; --- Check if prev is NULL (are we deleting the first node?) ---
    cmp  rbx, 0
    jne  delete_middle_node ; If prev is not NULL, jump to the logic for deleting a middle/end node.

delete_first_node:
    mov  [r13], r12         ; *begin_list = current->next. Update the head of the list.
    jmp  free_memory        ; Jump to the common memory free section.

delete_middle_node:
    mov  [rbx], r12         ; prev->next = current->next. Bypass the current node.

free_memory:
    push rax
    push r12
    push rbx                
    mov  rdi, [rax + 8]
    call rcx
    pop  rbx
    pop  r12
    pop  rax

    push rbx                
    mov  rdi, rax
    call free
    pop  rbx                

    mov  rax, r12
    jmp  loop_start         ; Go back to the start of the loop. 'prev' remains unchanged.

move_to_next:
    mov  rbx, rax           ; prev = current.
    mov  rax, [rax]         ; current = current->next.
    jmp  loop_start         ; Go back to the start of the loop.

end_function:
    add rsp, 8
    pop r13
    pop  r12                ; Restore the callee-saved registers from the stack.
    pop  rbx
    ret