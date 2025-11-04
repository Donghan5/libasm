section .text

global ft_atoi_base

; ------------------------------------------------
;- Register usage:
; 	rdi = *str (string to convert)
; 	rsi = *base (base string, from 2 to 16)
;	rax = result (converted integer)
; 	rbx = sign (0 = positive, 1 = negative)
; 	r12 = base_length (length of the base)
; 	r8  = index for str
; 	r9  = index for base
; ------------------------------------------------

; ------------------------------------------------
; 	ft_atoi_base - converts a string number in a given base to an integer
;	Parameters:
;		- rdi: pointer to the string to convert
;		- rsi: pointer to the base string (from 2 to 16)
;	Returns:
;		- rax: the converted integer value
; ------------------------------------------------
ft_atoi_base: ; rdi = *str(to convert), rsi = *base(2 to 16)
	push rbx     ; save rbx (sign)
	push r12     ; save r12 (base_length)
	xor rax, rax ; rax = 0 (result)
	xor rbx, rbx ; rbx = 0 (sign)
	xor r12, r12 ; r12 = 0 (base_length)
	jmp base_check_loop

; ---- Calculate base length and check for invalid characters ----
base_check_increment:
	inc r12      ; r12++ (base_length++)

; ---- Check for invalid characters in base ----
base_check_loop:
	cmp BYTE [rsi + r12], 0
	jz base_check_end
	mov r8, r12				; r8 = r12 (base_length), r8 is the length of base (temporary)
							; j = base_length

; ---- Check for duplicate characters in base ----
base_check_dup_inc:
	inc r8

; ---- Loop to check for duplicates ----
base_check_dup_loop:
	cmp BYTE [rsi + r8], 0  ; if (!base[j])
	jz base_check_correct
	mov r9b, [rsi + r8]
	cmp BYTE [rsi + r12], r9b
	je set_rax
	jmp base_check_dup_inc

; ---- Check for invalid characters in base ----
base_check_correct:
	cmp BYTE [rsi + r12], 32     ; to check if base[j] is space
	je set_rax
	cmp BYTE [rsi + r12], 9      ; to check if base[j] is tab
	je set_rax
	cmp BYTE [rsi + r12], 10     ; to check if base[j] is newline
	je set_rax
	cmp BYTE [rsi + r12], 13     ; to check if base[j] is return
	je set_rax
	cmp BYTE [rsi + r12], 11     ; to check if base[j] is vertical tab
	je set_rax
	cmp BYTE [rsi + r12], 12     ; to check if base[j] is form feed
	je set_rax
	cmp BYTE [rsi + r12], 43     ; to check if base[j] is +
	je set_rax
	cmp BYTE [rsi + r12], 45     ; to check if base[j] is -
	je set_rax
	jmp base_check_increment

; ---- End of base check ----
base_check_end:
	cmp r12, 1      ; base_length <= 1
	jle set_rax		; jump if less or equal to 1
	xor r8, r8		; r8 = 0 (index for str)
	jmp skip_whitespace

skip_whitespace_inc:        ; skip each whitespaces in str (using index)
	inc r8

skip_whitespace:            ; check each whitespaces in str
	cmp BYTE [rdi + r8], 32
	je skip_whitespace_inc
	cmp BYTE [rdi + r8], 9
	je skip_whitespace_inc
	cmp BYTE [rdi + r8], 10
	je skip_whitespace_inc
	cmp BYTE [rdi + r8], 13
	je skip_whitespace_inc
	cmp BYTE [rdi + r8], 11
	je skip_whitespace_inc
	cmp BYTE [rdi + r8], 12
	je skip_whitespace_inc
	jmp check_sign

is_negative:
	xor bl, 0x00000001

is_positive:
	inc r8

check_sign:
	cmp BYTE [rdi + r8], 43
	je is_positive
	cmp BYTE [rdi + r8], 45
	je is_negative
	jmp atoi_loop

atoi_increment:
	inc r8

; ---- Main conversion loop ----
atoi_loop:
	cmp BYTE [rdi + r8], 0      ; str[i] == 0
	je conversion_finished
	xor r9, r9				; r9 = 0 (index for base)
	jmp atoi_idx

atoi_idx_inc:
	inc r9

; ---- Loop to find the character in base ----
atoi_idx:
	mov r10b, BYTE [rsi + r9]
	cmp r10b, 0
	je conversion_finished
	cmp BYTE [rdi + r8], r10b    ; base[j] == str[i]
	jne atoi_idx_inc

; ---- If character found in base, add to total / Main calculation ----
add_to_total:
	mul r12			; rax = rax * base_length
	add rax, r9	; rax = rax + j (index in base)
	jmp atoi_increment

set_rax:
	xor rax, rax
	cmp rbx, 0
	jz return
	neg rax

conversion_finished:
	cmp rbx, 0
	jz return
	neg rax
	jmp return

return:
	pop r12
	pop rbx
	ret
