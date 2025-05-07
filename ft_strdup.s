section .text   ; declare the code segment

global _ft_strdup ; declare the function ft_strdup as global (exportable to other modules)

extern _malloc ; malloc is an external function (to allocate memory)

_ft_strdup:
	cmp rdi, 0 ; !rdi (if str is NULL)
	jz error ; go to error

len_starts:
	xor rcx, rcx ; rcx = 0 (index)
	jmp len_compare

len_increment:
	inc rcx ; rcx++

len_compare:
	cmp BYTE [rdi + rcx], 0 ; ![rdi + rcx] (if str[rcx] is NULL)
	jnz len_increment ; if not NULL, jump to len_increment

malloc_start:
	inc rcx ; rcx++ (increment length)
	push rdi ; save src
	mov rdi, rcx ; rdi = length  --- check the length of the string
	call _malloc ; allocate memory
	pop rdi ; restore src
	cmp rax, 0 ; if (!rax)
	jz error ; go to error jz == jump if zero

copy_start:
	xor rcx, rcx ; rcx = 0 (index)
	xor rdx, rdx ; rdx = 0 (temp)
	jmp copy_copy ; jump to copy_copy=

copy_increment:
	inc rcx ; rcx++

copy_copy:
	mov dl, BYTE [rdi + rcx] ; rdi = src[rcx]
	mov BYTE [rax + rcx], dl ; rax = dst[rcx]
	cmp dl, 0 ; !dl (null check)
	jnz copy_increment ; if not NULL, jump to copy_increment
	jmp return


error:
	xor rax, rax ; rax = 0 (return 0)

return:
	ret ; return the result
