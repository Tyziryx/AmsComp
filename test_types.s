			# This code was produced by the CERI Compiler
	.data
	.align 8
i:	.quad 0
d:	.double 0.0
c:	.byte 0
FormatString1:	.string "%llu\n"
FormatDouble:	.string "%f\n"
FormatChar:	.string "%c\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $65
	popq i(%rip)
	pushq i(%rip)
	popq %rax
	cvtsi2sdq %rax, %xmm0
	movsd %xmm0, d(%rip)
	pushq i(%rip)
	popq %rax
	movb %al, c(%rip)
	pushq i(%rip)
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
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
	movsbl c(%rip), %eax
	pushq %rax
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
