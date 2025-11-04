section .text

global ft_list_sort

; ------------------------------------------------
; void ft_list_sort(t_list **begin_list, int (*cmp)(void*, void*));
; - Register usage:
; 	rdi = t_list **begin_list (first argument)
; 	rsi = int (*cmp)(void*, void*) (second argument)
; ------------------------------------------------
ft_list_sort:
    cmp     rdi, 0              ; Check if begin_list is NULL
    je      .end
    mov     r12, [rdi]          ; r12 = *begin_list (the head of the list)
    cmp     r12, 0              ; Check if list is empty
    je      .end
    cmp     rsi, 0              ; Check if cmp function is NULL
    je      .end

; ---- Outer loop: bubble sort to swap the value ----
.outer_loop_start:
    mov     r13, 0              ; r13 = swapped flag. Set to 0 (false) at the start of each pass.
    mov     r8, r12             ; r8 = current_node. Always start the pass from the head.

; ---- Inner loop: traverse the list ----
.inner_loop:
    cmp     r8, 0               ; Make sure current node is not null
    je      .check_swapped
    mov     r9, [r8 + 8]        ; r9 = next_node
    cmp     r9, 0
    je      .check_swapped      ; Reached the end of the list for this pass.

    ; --- Save registers before calling external function ---
    push    rdi
    push    rsi
    push    r8
    push    r9
    push    r12
    push    r13

    ; --- Prepare arguments for cmp and call it ---
    mov     rdi, [r8]           ; arg1 = current_node->data
    mov     rsi, [r9]           ; arg2 = next_node->data
    call    QWORD [rsp + 32]    ; Call original cmp function address (it's on the stack)

    ; --- Restore registers ---
    pop     r13
    pop     r12
    pop     r9
    pop     r8
    pop     rsi
    pop     rdi

    cmp     eax, 0              ; Check the return value of cmp() / eax is the result of cmp (returned value)
    jle     .no_swap            ; If data is already in order (<= 0), skip the swap.

    ; --- Swap the data pointers ---
    mov     r10, [r8]
    mov     r11, [r9]
    mov     [r8], r11
    mov     [r9], r10
    mov     r13, 1              ; A swap was made, so set swapped flag to 1 (true).

.no_swap:
    mov     r8, [r8 + 8]        ; Move to the next node: current_node = current_node->next
    jmp     .inner_loop

.check_swapped:
    cmp     r13, 1              ; Was anything swapped in this entire pass?
    je      .outer_loop_start   ; If yes (flag is 1), the list is not yet sorted. Start another pass.

.end:
    ret                         ; If no swaps were made (flag is 0), the list is sorted. Return.