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
	pop a
	push $1
	pop i
For1:
	push $5
	pop %rax
	cmpq %rax, i
	jg EndFor1
	push a
	push i
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop a
	incq i
	jmp For1
EndFor1:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
