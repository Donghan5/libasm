section .text
global _ft_strcpy

_ft_strcpy:  ; rdi (Register destination index (in 64 bit)) = dst
			 ; rdi is the register that already set by the caller
			 ;rsi (Register source index (in 64 bit)) = src
	xor rcx, rcx ; rcx = 0 (index)
	xor rdx, rdx ; rdx = 0 (temp)
	cmp rsi, 0 ; !rsi (if src is NULL)
	jz return ; return 0
	jmp copy ; jump to copy

increment:
	inc rcx ; rcx++

copy:
	mov dl, BYTE [rsi + rcx] ; rsi = src[rcx]
	mov BYTE [rdi + rcx], dl ; rdx = dst[rcx]
	cmp dl, 0 ; !dl (null check)
	jnz increment ; if not null, jump to increment

return:
	mov rax, rdi ; return dst (standard return for strcpy)
	ret
