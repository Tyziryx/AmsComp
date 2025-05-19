			# This code was produced by the CERI Compiler
	.data
	.align 8
i:	.quad 0
j:	.quad 0
result:	.quad 0
FormatString1:	.string "%llu\n"
FormatDouble:	.string "%f\n"
FormatChar:	.string "%c\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $0
	popq result(%rip)
	push $1
	popq i(%rip)
For1:
	push $5
	pop %rax
	cmpq %rax, i(%rip)
	jg EndFor1
	pushq i(%rip)
	popq j(%rip)
While2:
	pushq j(%rip)
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai3	# If above
	push $0		# False
	jmp Suite3
Vrai3:	push $0xFFFFFFFFFFFFFFFF		# True
Suite3:
	pop %rax
	test %rax, %rax
	jz EndWhile2
	pushq result(%rip)
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq result(%rip)
	pushq j(%rip)
	push $1
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	popq j(%rip)
	jmp While2
EndWhile2:
	pushq result(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	incq i(%rip)
	jmp For1
EndFor1:
	pushq result(%rip)
	push $10
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai5	# If above
	push $0		# False
	jmp Suite5
Vrai5:	push $0xFFFFFFFFFFFFFFFF		# True
Suite5:
	pushq result(%rip)
	push $20
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai6	# If below
	push $0		# False
	jmp Suite6
Vrai6:	push $0xFFFFFFFFFFFFFFFF		# True
Suite6:
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# AND
	pop %rax
	test %rax, %rax
	jz Else4
	push $89
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndIf4
Else4:
	push $78
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf4:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
