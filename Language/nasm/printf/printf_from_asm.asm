
extern	printf
extern  scanf
global 	main


section	.data

__format:	db	'%d', 10, 0, '%d', 0
__number_buff:	dd	0000

section .text


in:
	push	rbp
	mov	rbp,	rsp

	mov	rdi,	__format + 4
	mov	rsi,	__number_buff
	call	scanf

	mov	rsp,	rbp
	pop	rbp

	ret


out:	
	push	rbp
	mov	rbp,	rsp
	
	mov	rdi,	__format
	mov	rax,	1
	call	printf

	mov	rsp,	rbp
	pop	rbp

	ret

main:
	call	code
	ret

mm	dq	0x6D6D6D6D6D6D6D48
code:
	push 3
	push 4
	pop 	qword [__number_buff]
	fild	qword [__number_buff]
	pop 	qword [__number_buff]
	fild	qword [__number_buff]
	fmul
	fistp qword [__number_buff]
	push qword [__number_buff]

	pop rsi
	call out

	times 	1000000 nop
	ret
