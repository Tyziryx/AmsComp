			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
b:	.quad 0
sum:	.quad 0
is_even:	.quad 0
is_pos:	.quad 0
d:	.double 0.0
c:	.byte 0
day:	.quad 0
FormatString1:	.string "%llu\n"
FormatDouble:	.string "%f\n"
FormatChar:	.string "%c\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $42
	popq a(%rip)
	push $10
	popq b(%rip)
	pushq a(%rip)
	pushq b(%rip)
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq sum(%rip)
	pushq sum(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq a(%rip)
	push $2
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rdx	# MOD
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	je Vrai1	# If equal
	push $0		# False
	jmp Suite1
Vrai1:	push $0xFFFFFFFFFFFFFFFF		# True
Suite1:
	popq is_even(%rip)
	pushq a(%rip)
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai2	# If above
	push $0		# False
	jmp Suite2
Vrai2:	push $0xFFFFFFFFFFFFFFFF		# True
Suite2:
	popq is_pos(%rip)
	pushq is_even(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq is_pos(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	.data
.LC1:	.double 3.14159
	.text
	subq $8, %rsp
	movsd .LC1(%rip), %xmm0
	movsd %xmm0, (%rsp)
	movsd (%rsp), %xmm0
	addq $8, %rsp
	movsd %xmm0, d(%rip)
	subq $8, %rsp
	movsd d(%rip), %xmm0
	movsd %xmm0, (%rsp)
	movsd (%rsp), %xmm0
	addq $8, %rsp
	subq $8, %rsp
	andq $-16, %rsp
	movq $FormatDouble, %rsi
	movl $1, %edi
	movl $1, %eax
	call __printf_chk@PLT
	leaq 8(%rsp), %rsp
	push $65
	popq %rax
	movb %al, c(%rip)
	movsbl c(%rip), %eax
	pushq %rax
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
Repeat3:
	pushq a(%rip)
	push $1
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	popq a(%rip)
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq a(%rip)
	push $40
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai4	# If below
	push $0		# False
	jmp Suite4
Vrai4:	push $0xFFFFFFFFFFFFFFFF		# True
Suite4:
	pop %rax
	test %rax, %rax
	jz Repeat3
EndRepeat3:
	push $3
	popq day(%rip)
	pushq day(%rip)
	popq %r10
	# Début du case
	cmpq $1, %r10
	je CaseLabel5_0
	jmp CaseNext5_1
CaseLabel5_0:
	push $76
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_1:
	cmpq $2, %r10
	je CaseLabel5_1
	jmp CaseNext5_2
CaseLabel5_1:
	push $77
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_2:
	cmpq $3, %r10
	je CaseLabel5_2
	jmp CaseNext5_3
CaseLabel5_2:
	push $87
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_3:
	cmpq $4, %r10
	je CaseLabel5_3
	jmp CaseNext5_4
CaseLabel5_3:
	push $84
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_4:
	cmpq $5, %r10
	je CaseLabel5_4
	jmp CaseNext5_5
CaseLabel5_4:
	push $70
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_5:
	cmpq $6, %r10
	je CaseLabel5_5
	cmpq $7, %r10
	je CaseLabel5_5
	jmp CaseNext5_6
CaseLabel5_5:
	push $83
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_6:
EndCase5:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
