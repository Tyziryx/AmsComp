			# This code was produced by the CERI Compiler
	.data
	.align 8
day:	.quad 0
grade:	.byte 0
FormatString1:	.string "%llu\n"
FormatDouble:	.string "%f\n"
FormatChar:	.string "%c\n"
	.text		# The following lines contain the program
	.globl main	# The main function must be visible from outside
main:			# The main function body :
	movq %rsp, %rbp	# Save the position of the stack's top
	push $3
	popq day(%rip)
	pushq day(%rip)
	popq %r10
	# Début du case
	cmpq $1, %r10
	je CaseLabel1_0
	jmp CaseNext1_1
CaseLabel1_0:
	push $77
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_1:
	cmpq $2, %r10
	je CaseLabel1_1
	jmp CaseNext1_2
CaseLabel1_1:
	push $84
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_2:
	cmpq $3, %r10
	je CaseLabel1_2
	jmp CaseNext1_3
CaseLabel1_2:
	push $87
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_3:
	cmpq $4, %r10
	je CaseLabel1_3
	jmp CaseNext1_4
CaseLabel1_3:
	push $84
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_4:
	cmpq $5, %r10
	je CaseLabel1_4
	jmp CaseNext1_5
CaseLabel1_4:
	push $70
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_5:
	cmpq $6, %r10
	je CaseLabel1_5
	cmpq $7, %r10
	je CaseLabel1_5
	jmp CaseNext1_6
CaseLabel1_5:
	push $83
	popq %rdx
	movq $FormatChar, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase1
CaseNext1_6:
EndCase1:
	push $66
	popq %rax
	movb %al, grade(%rip)
	movsbl grade(%rip), %eax
	pushq %rax
	popq %r10
	# Début du case
	cmpq $65, %r10
	je CaseLabel2_0
	jmp CaseNext2_1
CaseLabel2_0:
	push $90
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase2
CaseNext2_1:
	cmpq $66, %r10
	je CaseLabel2_1
	jmp CaseNext2_2
CaseLabel2_1:
	push $80
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase2
CaseNext2_2:
	cmpq $67, %r10
	je CaseLabel2_2
	jmp CaseNext2_3
CaseLabel2_2:
	push $70
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase2
CaseNext2_3:
	cmpq $68, %r10
	je CaseLabel2_3
	cmpq $70, %r10
	je CaseLabel2_3
	jmp CaseNext2_4
CaseLabel2_3:
	push $60
	popq %rdx
	movq $FormatString1, %rsi
	movl $1, %edi
	movl $0, %eax
	call __printf_chk@PLT
	jmp EndCase2
CaseNext2_4:
EndCase2:
	movq %rbp, %rsp		# Restore the position of the stack's top
	ret			# Return from main function
