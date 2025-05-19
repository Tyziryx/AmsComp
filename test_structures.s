			# This code was produced by the CERI Compiler
	.data
	.align 8
x:	.quad 0
somme:	.quad 0
c:	.byte 0
FormatString1:	.string "%llu\n"
FormatDouble:	.string "%f\n"
FormatChar:	.string "%c\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $0
	popq x(%rip)
	push $0
	popq somme(%rip)
Repeat1:
	pushq somme(%rip)
	pushq x(%rip)
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq somme(%rip)
	pushq x(%rip)
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	popq x(%rip)
	pushq somme(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	pushq x(%rip)
	push $5
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai2	# If above
	push $0		# False
	jmp Suite2
Vrai2:	push $0xFFFFFFFFFFFFFFFF		# True
Suite2:
	pop %rax
	test %rax, %rax
	jz Repeat1
EndRepeat1:
	push $67
	popq %rax
	movb %al, c(%rip)
	movsbl c(%rip), %eax
	pushq %rax
	popq %r10
	# Début du case
	cmpq $65, %r10
	je CaseLabel3_0
	jmp CaseNext3_1
CaseLabel3_0:
	push $1
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase3
CaseNext3_1:
	cmpq $66, %r10
	je CaseLabel3_1
	cmpq $67, %r10
	je CaseLabel3_1
	cmpq $68, %r10
	je CaseLabel3_1
	jmp CaseNext3_2
CaseLabel3_1:
	push $2
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase3
CaseNext3_2:
	cmpq $69, %r10
	je CaseLabel3_2
	jmp CaseNext3_3
CaseLabel3_2:
	push $3
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase3
CaseNext3_3:
EndCase3:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
