section .text  ; code segment
global _ft_read ; declare the function (to export it in the other modules)
			   ; if the linux, it will be ft_read(check it)

_ft_read:
	mov rax, 0x2000003 ; syscall number for read (0 for linux)
	syscall ; call the kernel
	ret
