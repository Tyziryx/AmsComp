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
	pushq a(%rip)
	popq %r10
	# Début du case
	cmpq $38, %r10
	je CaseLabel5_0
	jmp CaseNext5_1
CaseLabel5_0:
	push $65
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_1:
	cmpq $39, %r10
	je CaseLabel5_1
	jmp CaseNext5_2
CaseLabel5_1:
	push $66
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_2:
	cmpq $40, %r10
	je CaseLabel5_2
	jmp CaseNext5_3
CaseLabel5_2:
	push $67
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_3:
	cmpq $41, %r10
	je CaseLabel5_3
	cmpq $42, %r10
	je CaseLabel5_3
	jmp CaseNext5_4
CaseLabel5_3:
	push $68
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase5
CaseNext5_4:
EndCase5:
	push $88
	popq %rax
	movb %al, c(%rip)
	movsbl c(%rip), %eax
	pushq %rax
	popq %r10
	# Début du case
	cmpq $65, %r10
	je CaseLabel6_0
	jmp CaseNext6_1
CaseLabel6_0:
	push $1
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase6
CaseNext6_1:
	cmpq $66, %r10
	je CaseLabel6_1
	jmp CaseNext6_2
CaseLabel6_1:
	push $2
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase6
CaseNext6_2:
	cmpq $88, %r10
	je CaseLabel6_2
	jmp CaseNext6_3
CaseLabel6_2:
	push $3
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase6
CaseNext6_3:
	cmpq $89, %r10
	je CaseLabel6_3
	cmpq $90, %r10
	je CaseLabel6_3
	jmp CaseNext6_4
CaseLabel6_3:
	push $4
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase6
CaseNext6_4:
EndCase6:
	pushq is_pos(%rip)
	pushq is_even(%rip)
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# AND
	pop %rax
	test %rax, %rax
	jz Else7
	pushq a(%rip)
	push $2
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
	jmp EndIf7
Else7:
	pushq a(%rip)
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq a(%rip)
	pushq a(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf7:
	push $0
	popq a(%rip)
While8:
	pushq a(%rip)
	push $3
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai9	# If below
	push $0		# False
	jmp Suite9
Vrai9:	push $0xFFFFFFFFFFFFFFFF		# True
Suite9:
	pop %rax
	test %rax, %rax
	jz EndWhile8
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
	jmp While8
EndWhile8:
	push $1
	popq a(%rip)
For10:
	push $3
	pop %rax
	cmpq %rax, a(%rip)
	jg EndFor10
	pushq a(%rip)
	popq %rax
	cvtsi2sdq %rax, %xmm0
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
	incq a(%rip)
	jmp For10
EndFor10:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
