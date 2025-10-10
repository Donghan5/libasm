section .text

global ft_write

extern __errno_location

ft_write:
	cmp rsi, 0		; check if buf is NULL
	je .error
	
    mov rax, 1      ; syscall number for sys_write
    syscall
    cmp rax, 0      ; Check if the return value is negative
    jl  .error      ; If it is, jump to the error handling part
    ret

.error:
    neg rax                     ; Negate rax to get the positive errno value
    mov rdi, rax                ; Move the errno value to rdi
    call __errno_location wrt ..plt ; Get the address of errno
    mov [rax], rdi              ; Set errno
    mov rax, -1                 ; Set the return value to -1
    ret