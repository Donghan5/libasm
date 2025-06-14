section .text  ; declare the code segment --> indicate that this is the code section
global ft_write  ; declare the function ft_write as global (exportable to other modules)

ft_write:   	  ; start of the function
	mov rax, 1 ; syscall number for sys_write 0x2000004 is for 64-bit (macOS), Linux uses different (use 1)
	syscall  	   ; invoke syscall
	ret     ; return
