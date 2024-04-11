	.text
	.globl _hashfunc
_hashfunc:

LFB0:
	// base pointer to stack
	pushq	%rbp
LCFI0:
	// stack pointer to base pointer
	movq	%rsp, %rbp
LCFI1:
	// stackpointer -8 (why 8 bit? - I think its padding)
	subq	$8, %rsp
	// rdi ? 
	// 120 bits for 26 (shorts?)
	movq	%rdi, -120(%rbp)
	movl	$10, -112(%rbp)
	movl	$42, -108(%rbp)
	movl	$12, -104(%rbp)
	movl	$21, -100(%rbp)
	movl	$7, -96(%rbp)
	movl	$5, -92(%rbp)
	movl	$67, -88(%rbp)
	movl	$48, -84(%rbp)
	movl	$69, -80(%rbp)
	movl	$2, -76(%rbp)
	movl	$36, -72(%rbp)
	movl	$3, -68(%rbp)
	movl	$19, -64(%rbp)
	movl	$1, -60(%rbp)
	movl	$14, -56(%rbp)
	movl	$51, -52(%rbp)
	movl	$71, -48(%rbp)
	movl	$8, -44(%rbp)
	movl	$26, -40(%rbp)
	movl	$54, -36(%rbp)
	movl	$75, -32(%rbp)
	movl	$15, -28(%rbp)
	movl	$6, -24(%rbp)
	movl	$59, -20(%rbp)
	movl	$13, -16(%rbp)
	movl	$25, -12(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	// padding here
	jmp	L2
L6:
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	// TODO %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$64, %al
	jle	L3
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$90, %al
	jg	L3
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$65, %eax
	cltq
	movl	-112(%rbp,%rax,4), %eax
	addl	%eax, -4(%rbp)
	jmp	L4
L3:
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$96, %al
	jle	L5
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$122, %al
	jg	L5
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$97, %eax
	cltq
	movl	-112(%rbp,%rax,4), %eax
	subl	%eax, -4(%rbp)
	jmp	L4
L5:
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	jle	L4
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$57, %al
	jg	L4
	movl	-8(%rbp), %eax
	cltq
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	addl	%eax, -4(%rbp)
L4:
	addl	$1, -8(%rbp)
L2:
	// Base pointer -8 to %eax
	movl	-8(%rbp), %eax
	cltq
	// Base pointer -120 to %rdx
	// %rdx - %eax: our array
	movq	-120(%rbp), %rdx
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	L6
	movl	-4(%rbp), %eax
	leave
LCFI2:
	ret
LFE0:
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0
	.byte	0x1
	.ascii "zR\0"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x10
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 3
LECIE1:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB0-.
	.set L$set$2,LFE0-LFB0
	.quad L$set$2
	.uleb128 0
	.byte	0x4
	.set L$set$3,LCFI0-LFB0
	.long L$set$3
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.set L$set$5,LCFI2-LCFI1
	.long L$set$5
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.align 3
LEFDE1:
	.ident	"GCC: (Homebrew GCC 13.2.0) 13.2.0"
	.subsections_via_symbols
