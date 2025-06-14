section .text  ; code segment

global ft_read ; declare the function (to export it in the other modules)
			   ; if the linux, it will be ft_read(check it)

ft_read:
	mov rax, 0 ; syscall number for read (0 for linux)
	syscall ; call the kernel
	ret
