section .text  ; code segment

global ft_read ; declare the function (to export it in the other modules)
			   ; if the linux, it will be ft_read(check it)

extern __errno_location

ft_read:
	mov rax, 0 	; syscall number for read (0 for linux)
	syscall 	; call the kernel
	cmp rax, 0
	jl .error	; jl = jump if less
	ret

.error:
	neg rax								; negate rax to get the positive errno
	mov rdi, rax						; move errno val to rdi
	call __errno_location wrt ..plt		; get the address of errno
	mov [rax], rdi						; setting errno
	mov rax, -1							; set return value to -1
	ret