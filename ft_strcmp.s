section .text

global ft_strcmp

; delete RDX, RCX, RAX (it means that these registers will be modified)

ft_strcmp:       ; s1 = rdi, s2 = rsi
	xor rcx, rcx  ; rcx = 0 (index)
	xor rdx, rdx  ; rdx = 0 (result)
	cmp rdi, 0
	jz check_null ; if s1 is NULL, jump to check_null
	cmp rsi, 0
	jz check_null ; if s2 is NULL, jump to check_null
	jmp check

check_null:
	cmp rdi, rsi
	jz equal  ; if s1 == s2 == NULL
	jg superior  ; if s1 == NULL
	jmp inferior ; if s2 == NULL

compare:
	mov dl, BYTE [rsi + rcx]  ; tmp = s2[rcx (index)]
	cmp BYTE [rdi + rcx], dl  ; s[rcx(index)] == tmp
	jne last_char  ; if s1[rcx] != s2[rcx], jump to last_char

increment:
	inc rcx  ; rcx++

check:
	cmp BYTE [rdi + rcx], 0  ; s1[rcx] == NULL
	je last_char  ; if s1[rcx] == NULL, jump to last_char
	cmp BYTE [rsi + rcx], 0 ; s2[rcx] == NULL
	je last_char  ; if s2[rcx] == NULL, jump to last_char
	jmp compare  ; jump to compare

last_char:
	mov dl, BYTE [rdi + rcx] ; dl = s1[rcx]
	sub dl, BYTE [rsi + rcx] ; dl = dl - s2[rcx]
	cmp dl, 0 ; dl (result) == 0 ? (compare the result)
	jz equal  ; if dl == 0, jump to equal
	jl inferior ; if dl < 0, jump to inferior

superior:
	mov rax, 1  ; rax = 1 (set 1 to rax register)
	ret

inferior:
	mov rax, -1  ; rax = -1 (set -1 to rax register)
	ret

equal:
	mov rax, 0  ; rax = 0 (set 0 to rax register)
	ret
