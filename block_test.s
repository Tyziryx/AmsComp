			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
b:	.quad 0
c:	.quad 0
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $1
	pop a
	push $2
	pop b
	push a
	push b
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop c
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
