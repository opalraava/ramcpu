	# this is a comment
	nop #this is a comment
	nop#another
	nop;

	.ascii "Hello, world!\12\0" 
	.string "This is another string"
	.org $100

;;; more on constants here: http://tigcc.ticalc.org/doc/gnuasm.html#SEC32
	.byte 4, 0112, 092, 0x4A, 0X4a, 'J ;# All the same value. 
	.ascii "Ring the bell\7"                  ;# A string constant.
	.octa	0x123456789abcdef0123456789ABCDEF0 ;# A bignum.
	.float	0f-3141592653589793238462643383275028841971.693993751E-40                 ;# - pi, a flonum.
	
	;; INTEL VS AT&T SYNTAX: http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm

	;; constants
	mov	$077,   %ax		;octal
	mov	$0xef ,%eax		;hex
	mov	$-34 , %eax		;decimal
	mov	$0b10101010,%eax	;binary
	movl    -4(%ebp, %edx, 4), %eax;# Full example: load *(ebp - 4 + (edx * 4)) into eax
	movl    -4(%ebp), %eax           ;# Typical example: load a stack variable into eax
	movl    (%ecx), %edx             ;# No offset: copy the target of a pointer into a register
	leal    8(,%eax,4), %eax         ;# Arithmetic: multiply eax by 4 and add 8
	leal    (%edx,%eax,2), %eax# Arithmetic: multiply eax by 2 and add edx


_main:
	pushl   %ebp
	movl    %esp, %ebp
	subl    $8, %esp
	andl    $-16, %esp
	movl    $0, %eax
	movl    %eax, -4(%ebp)
	movl    -4(%ebp), %eax
	call    __alloca
	call    ___main
	movl    $LC0, (%esp)
	call    _printf
	movl    $0, %eax
	leave
	ret

garbage:	
	
	movl    $1,%eax
	movl    $0xff,%ebx
	int     $0x80
	movl    (%ecx),%eax
	movl    (%ebx),%eax
	movl    3(%ebx),%eax			; mov     eax,[ebx+3]
	
	movl    0x20(%ebx),%eax		     	; mov     eax,[ebx+20h]
	addl    (%ebx,%ecx,0x2),%eax	     	; add     eax,[ebx+ecx*2h]
	leal    (%ebx,%ecx),%eax	     	; lea     eax,[ebx+ecx]
	subl    -0x20(%ebx,%ecx,0x4),%eaxsub 	; eax,[ebx+ecx*4h-20h]

	;; suffixes b, w, l
	movb    %bl,%al
	movw    %bx,%ax
	movl    %ebx,%eax
	movl    (%ebx),%eax


;;; output from cc -S as.c stripped of .garbage


	pushq	%rbp
	movq	%rsp, %rbp
	movl	$.LC0, %edi
	call	puts
	movl	$1, %edi
	call	exit
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	cmpl	$4, -20(%rbp)
	je	.L3
	call	usage
.L3:
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L4
	call	usage
.L4:
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	.L5
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	perror
	movl	$1, %eax
	jmp	.L6
.L5:
	movq	-32(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rax
	movl	$.LC3, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -16(%rbp)
	cmpq	$0, -16(%rbp)
	jne	.L7
	movq	-32(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	perror
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$1, %eax
	jmp	.L6
.L7:
	movq	-32(%rbp), %rax
	addq	$24, %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	as
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	movl	$0, %eax
.L6:
	leave
	ret
.LFE3:
.LC4:
syntax_error:
.LFB4:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rcx
	movl	-12(%rbp), %edx
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	nop
	leave
	ret
.LFE4:
.LC5:
.LC6:
as:
.LFB5:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$656, %rsp
	movq	%rdi, -632(%rbp)
	movq	%rsi, -640(%rbp)
	movq	%rdx, -648(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L10
.L35:
	addl	$1, -4(%rbp)
	leaq	-560(%rbp), %rax
	movq	%rax, %rdi
	call	trim
	movzbl	-560(%rbp), %eax
	testb	%al, %al
	je	.L10
	movzbl	-560(%rbp), %eax
	cmpb	$59, %al
	jne	.L12
	jmp	.L10
.L12:
	movq	$0, -592(%rbp)
	movq	-592(%rbp), %rax
	movq	%rax, -600(%rbp)
	movq	-600(%rbp), %rax
	movq	%rax, -608(%rbp)
	movq	-608(%rbp), %rax
	movq	%rax, -616(%rbp)
	movq	-616(%rbp), %rax
	movq	%rax, -624(%rbp)
	leaq	-560(%rbp), %rax
	movq	%rax, -16(%rbp)
	nop
	movq	-16(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L10
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
	leaq	-568(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	try_parse_identifier
	testl	%eax, %eax
	jne	.L14
	movq	-568(%rbp), %rax
	movq	%rax, -24(%rbp)
	jmp	.L15
.L17:
	addq	$1, -24(%rbp)
.L15:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L16
	call	__ctype_b_loc
	movq	(%rax), %rdx
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L17
.L16:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$58, %al
	jne	.L14
	movq	-24(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -24(%rbp)
	movb	$0, (%rax)
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	call	dupstr
	movq	%rax, -624(%rbp)
	jmp	.L18
.L20:
	addq	$1, -24(%rbp)
.L18:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L19
	call	__ctype_b_loc
	movq	(%rax), %rdx
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L20
.L19:
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L21
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$59, %al
	jne	.L22
.L21:
	movl	-4(%rbp), %edx
	movq	-648(%rbp), %rcx
	leaq	-624(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	process_instruction
	nop
	jmp	.L10
.L22:
	movq	-24(%rbp), %rax
	movq	%rax, -16(%rbp)
.L14:
	movq	-16(%rbp), %rax
	movq	%rax, -48(%rbp)
	leaq	-576(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	try_parse_instruction
	testl	%eax, %eax
	jne	.L36
	movq	-576(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L27
	call	__ctype_b_loc
	movq	(%rax), %rdx
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L27
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$59, %al
	je	.L27
	movl	-4(%rbp), %ecx
	movq	-648(%rbp), %rax
	movl	$.LC5, %edx
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	syntax_error
	movl	$1, %edi
	call	exit
.L27:
	movq	-32(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -32(%rbp)
	movb	$0, (%rax)
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	dupstr
	movq	%rax, -616(%rbp)
	jmp	.L28
.L30:
	addq	$1, -32(%rbp)
.L28:
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L29
	call	__ctype_b_loc
	movq	(%rax), %rdx
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	movsbq	%al, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	jne	.L30
.L29:
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L31
	movq	-32(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$59, %al
	jne	.L32
.L31:
	movl	-4(%rbp), %edx
	movq	-648(%rbp), %rcx
	leaq	-624(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	process_instruction
	nop
	jmp	.L10
.L32:
	movq	-32(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	dupstr
	movq	%rax, -608(%rbp)
	movl	-4(%rbp), %edx
	movq	-648(%rbp), %rcx
	leaq	-624(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	process_instruction
	jmp	.L10
.L36:
	movl	-4(%rbp), %ecx
	movq	-648(%rbp), %rax
	movl	$.LC6, %edx
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	syntax_error
	movl	$1, %edi
	call	exit
.L10:
	movq	-632(%rbp), %rdx
	leaq	-560(%rbp), %rax
	movl	$512, %esi
	movq	%rax, %rdi
	call	fgets
	testq	%rax, %rax
	jne	.L35
	nop
	leave
	ret
.LFE5:
.LC7:
.LC8:
dump_line_parts:
.LFB6:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movq	-8(%rbp), %rax
	movq	32(%rax), %rax
	testq	%rax, %rax
	je	.L38
	movq	-8(%rbp), %rax
	movq	32(%rax), %rcx
	jmp	.L39
.L38:
	movl	$.LC7, %ecx
.L39:
	movq	-8(%rbp), %rax
	movq	24(%rax), %rax
	testq	%rax, %rax
	je	.L40
	movq	-8(%rbp), %rax
	movq	24(%rax), %rdx
	jmp	.L41
.L40:
	movl	$.LC7, %edx
.L41:
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	testq	%rax, %rax
	je	.L42
	movq	-8(%rbp), %rax
	movq	16(%rax), %r9
	jmp	.L43
.L42:
	movl	$.LC7, %r9d
.L43:
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	testq	%rax, %rax
	je	.L44
	movq	-8(%rbp), %rax
	movq	8(%rax), %r8
	jmp	.L45
.L44:
	movl	$.LC7, %r8d
.L45:
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	je	.L46
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	jmp	.L47
.L46:
	movl	$.LC7, %eax
.L47:
	movl	-20(%rbp), %edi
	movq	-16(%rbp), %rsi
	pushq	%rcx
	pushq	%rdx
	movq	%rax, %rcx
	movl	%edi, %edx
	movl	$.LC8, %edi
	movl	$0, %eax
	call	printf
	addq	$16, %rsp
	nop
	leave
	ret
.LFE6:
process_instruction:
.LFB7:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	%edx, -20(%rbp)
	movl	-20(%rbp), %edx
	movq	-16(%rbp), %rcx
	movq	-8(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	dump_line_parts
	nop
	leave
	ret
.LFE7:


THE_END:	STOP
