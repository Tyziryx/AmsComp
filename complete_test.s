			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
b:	.quad 0
c:	.quad 0
is_even:	.quad 0
is_positive:	.quad 0
sum:	.quad 0
FormatString1:	.string "%llu\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $10
	pop a
	push $5
	pop b
	push $0
	pop c
	push $0
	pop sum
	push a
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push b
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push a
	push b
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop c
	push c
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push c
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
	pop is_even
	push c
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai2	# If above
	push $0		# False
	jmp Suite2
Vrai2:	push $0xFFFFFFFFFFFFFFFF		# True
Suite2:
	pop is_positive
	push is_even
	push is_positive
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# AND
	pop %rax
	test %rax, %rax
	jz Else3
	push $100
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push a
	push $2
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	pop a
	jmp EndIf3
Else3:
	push $200
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf3:
	push a
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $1
	pop a
While4:
	push a
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
	push sum
	push a
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop sum
	push a
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop a
	jmp While4
EndWhile4:
	push sum
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $1
For6:
	push $3
	pop %rax
	cmpq %rax, b
	jg EndFor6
	push b
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push sum
	push b
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop sum
	incq b
	jmp For6
EndFor6:
	push sum
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	push $0xFFFFFFFFFFFFFFFF	# True
	pop %rax
	test %rax, %rax
	jz Else7
	push $42
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndIf7
Else7:
	push $0
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf7:
	push $0	# False
	pop %rax
	test %rax, %rax
	jz Else8
	push $0
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndIf8
Else8:
	push $99
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
EndIf8:
	push $9999
	pop %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
