			# This code was produced by the CERI Compiler
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $5
	push $3
	pop %rbx
	pop %rax
	addq	%rbx, %rax
	push %rax
	push $10
