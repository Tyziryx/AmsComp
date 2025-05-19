			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
b:	.quad 0
sum:	.quad 0
is_even:	.quad 0
is_positive:	.quad 0
pi:	.double 0.0
radius:	.double 0.0
area:	.double 0.0
first_letter:	.byte 0
next_letter:	.byte 0
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
	pushq sum(%rip)
	pushq b(%rip)
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
	push $2
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	popq b(%rip)
	pushq b(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq b(%rip)
	push $4
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rax	# DIV
	popq a(%rip)
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq a(%rip)
	push $3
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rdx	# MOD
	popq b(%rip)
	pushq b(%rip)
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
	popq is_positive(%rip)
	pushq is_even(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq is_positive(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq is_even(%rip)
	pushq is_positive(%rip)
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# AND
	popq is_positive(%rip)
	pushq is_positive(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $0	# False
	push $0xFFFFFFFFFFFFFFFF	# True
	pop %rbx
	pop %rax
	addq %rbx, %rax	# OR
	push %rax
	popq is_positive(%rip)
	pushq is_positive(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $0	# False
	pop %rax
	not %rax
	push %rax	# NOT
	popq is_positive(%rip)
	pushq is_positive(%rip)
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
	movsd %xmm0, pi(%rip)
	push $5
	popq %rax
	cvtsi2sdq %rax, %xmm0
	movsd %xmm0, radius(%rip)
	.data
.LC2:	.double 78.5397
	.text
	subq $8, %rsp
	movsd .LC2(%rip), %xmm0
	movsd %xmm0, (%rsp)
	movsd (%rsp), %xmm0
	addq $8, %rsp
	movsd %xmm0, area(%rip)
	subq $8, %rsp
	movsd pi(%rip), %xmm0
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
	subq $8, %rsp
	movsd radius(%rip), %xmm0
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
	subq $8, %rsp
	movsd area(%rip), %xmm0
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
	movb %al, first_letter(%rip)
	movsbl first_letter(%rip), %eax
	pushq %rax
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $66
	popq %rax
	movb %al, next_letter(%rip)
	movsbl next_letter(%rip), %eax
	pushq %rax
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $1
	popq a(%rip)
	pushq is_positive(%rip)
	pop %rax
	test %rax, %rax
	jz Else3
	pushq a(%rip)
	push $10
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	popq a(%rip)
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndIf3
Else3:
	push $0
	popq a(%rip)
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf3:
	push $0
	popq a(%rip)
While4:
	pushq a(%rip)
	push $5
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai5	# If below
	push $0		# False
	jmp Suite5
Vrai5:	push $0xFFFFFFFFFFFFFFFF		# True
Suite5:
	pop %rax
	test %rax, %rax
	jz EndWhile4
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq a(%rip)
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq a(%rip)
	jmp While4
EndWhile4:
	push $1
	popq a(%rip)
For6:
	push $3
	pop %rax
	cmpq %rax, a(%rip)
	jg EndFor6
	pushq a(%rip)
	push $2
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	popq b(%rip)
	pushq b(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq a(%rip)
	popq %rax
	cvtsi2sdq %rax, %xmm0
	movsd %xmm0, radius(%rip)
	subq $8, %rsp
	movsd radius(%rip), %xmm0
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
	pushq a(%rip)
	push $64
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq %rax
	movb %al, next_letter(%rip)
	movsbl next_letter(%rip), %eax
	pushq %rax
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	incq a(%rip)
	jmp For6
EndFor6:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
