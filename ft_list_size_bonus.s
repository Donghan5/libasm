section .text

global ft_list_size

; grammatical notions --> [] is like * (in c or c++), lt's means that pointer!
; int ft_list_size(t_list *begin_list);
; rdi: begin_list, returns length in rax
ft_list_size:
    xor rax, rax          ; rax = 0 (length counter)
    cmp rdi, 0            ; if (begin_list == NULL)
    jz return_end         ;   return 0
    jmp loop_start        ; Start the loop

increment_and_next:
    inc rax               ; length++
    mov rdi, [rdi]        ; rdi = rdi->next (Move to the next node)

loop_start:
    cmp rdi, 0            ; while (rdi != NULL)
    jnz increment_and_next  ; if not NULL, increment count and move to the next node

return_end:
    ret                   ; Return rax