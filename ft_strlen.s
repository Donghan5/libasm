section .text         ; declare the code segment
global ft_strlen      ; declare the function ft_strlen as global

ft_strlen:            ; start of the func
                      ; rdi register holds the pointer to the string
    xor rax, rax      ; rax init to 0 (length counter)

.loop:
    cmp BYTE [rdi + rax], 0 ; current character (pointed by rdi + rax) with 0
    je .done          ; if it is 0, jump to .done
    inc rax           ; increment rax (length counter)
    jmp .loop         ; ; jump to .loop

.done:
    ret               ; exit function
					  ; return to the caller
