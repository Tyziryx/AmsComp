			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
i:	.quad 0
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $0
	pop i
While1:
	push i
	push $5
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai2	# If below
	push $0		# False
	jmp Suite2
Vrai2:	push $0xFFFFFFFFFFFFFFFF		# True
Suite2:
	pop %rax
	test %rax, %rax
	jz EndWhile1
	push i
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop i
	jmp While1
EndWhile1:
	push i
	pop a
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
