			# This code was produced by the CERI Compiler
	.data
	.align 8
a:	.quad 0
b:	.quad 0
c:	.quad 0
d:	.quad 0
e:	.quad 0
i:	.quad 0
j:	.quad 0
result:	.quad 0
temp:	.quad 0
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $100
	pop a
	push $7
	pop b
	push $3
	pop c
	push a
	push b
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rax	# DIV
	push b
	push c
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	push a
	push b
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rdx	# MOD
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	pop d
	push $0
	pop a
	push $5
	pop i
While1:
	push i
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
	jz EndWhile1
	push a
	push i
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop a
	push a
	push $10
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai4	# If above
	push $0		# False
	jmp Suite4
Vrai4:	push $0xFFFFFFFFFFFFFFFF		# True
Suite4:
	pop %rax
	test %rax, %rax
	jz Else3
	push $0
	pop i
	jmp EndIf3
Else3:
	push i
	push $1
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	pop i
EndIf3:
	jmp While1
EndWhile1:
	push $0
	pop result
	push $1
For5:
	push a
	pop %rax
	cmpq %rax, i
	jg EndFor5
	push i
	push b
	pop %rbx
	pop %rax
	movq $0, %rdx
	div %rbx
	push %rdx	# MOD
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	je Vrai7	# If equal
	push $0		# False
	jmp Suite7
Vrai7:	push $0xFFFFFFFFFFFFFFFF		# True
Suite7:
	pop %rax
	test %rax, %rax
	jz Else6
	push result
	push i
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop result
	jmp EndIf6
Else6:
EndIf6:
	push result
	push $50
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai9	# If above
	push $0		# False
	jmp Suite9
Vrai9:	push $0xFFFFFFFFFFFFFFFF		# True
Suite9:
	pop %rax
	test %rax, %rax
	jz Else8
	push a
	pop i
	jmp EndIf8
Else8:
EndIf8:
	incq i
	jmp For5
EndFor5:
	push a
	push a
	pop %rbx
	pop %rax
	mulq	%rbx
	push %rax	# MUL
	pop result
	push result
	push $100
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai11	# If above
	push $0		# False
	jmp Suite11
Vrai11:	push $0xFFFFFFFFFFFFFFFF		# True
Suite11:
	pop %rax
	test %rax, %rax
	jz Else10
	push $100
	pop result
	jmp EndIf10
Else10:
EndIf10:
	push $5
	pop a
	push $0
	pop i
While12:
	push i
	push a
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	jb Vrai13	# If below
	push $0		# False
	jmp Suite13
Vrai13:	push $0xFFFFFFFFFFFFFFFF		# True
Suite13:
	pop %rax
	test %rax, %rax
	jz EndWhile12
	push i
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
	je Vrai15	# If equal
	push $0		# False
	jmp Suite15
Vrai15:	push $0xFFFFFFFFFFFFFFFF		# True
Suite15:
	pop %rax
	test %rax, %rax
	jz Else14
	push $1
For16:
	push i
	pop %rax
	cmpq %rax, j
	jg EndFor16
	push result
	push j
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop result
	incq j
	jmp For16
EndFor16:
	jmp EndIf14
Else14:
	push i
	pop j
While17:
	push j
	push $0
	pop %rax
	pop %rbx
	cmpq %rax, %rbx
	ja Vrai18	# If above
	push $0		# False
	jmp Suite18
Vrai18:	push $0xFFFFFFFFFFFFFFFF		# True
Suite18:
	pop %rax
	test %rax, %rax
	jz EndWhile17
	push result
	push j
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	pop result
	push j
	push $1
	pop %rbx
	pop %rax
	subq %rbx, %rax	# SUB
	push %rax
	pop j
	jmp While17
EndWhile17:
EndIf14:
	push i
	push $1
	pop %rbx
	pop %rax
	addq %rbx, %rax	# ADD
	push %rax
	pop i
	jmp While12
EndWhile12:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
