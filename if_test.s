			# This code was produced by the CERI Compiler
	.data
	.align 8
x:	.quad 0
y:	.quad 0
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push x
	push $0
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
	jz Else1
	push $1
	pop y
	jmp EndIf1
Else1:
	push $0
	pop y
EndIf1:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
