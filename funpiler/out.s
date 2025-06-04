	.text
	.file	"out.ll"
	.globl	uart_write                      // -- Begin function uart_write
	.p2align	2
	.type	uart_write,@function
uart_write:                             // @uart_write
	.cfi_startproc
// %bb.0:                               // %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #150994944                  // =0x9000000
	str	x0, [sp, #8]
.LBB0_1:                                // %loop
                                        // =>This Inner Loop Header: Depth=1
	ldr	x9, [sp, #8]
	ldrb	w10, [x9]
	cbz	w10, .LBB0_3
// %bb.2:                               // %send
                                        //   in Loop: Header=BB0_1 Depth=1
	add	x9, x9, #1
	str	w10, [x8]
	str	x9, [sp, #8]
	b	.LBB0_1
.LBB0_3:                                // %done
	add	sp, sp, #16
	ret
.Lfunc_end0:
	.size	uart_write, .Lfunc_end0-uart_write
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:                               // %entry
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -16
	adrp	x0, :got:str1
	ldr	x0, [x0, :got_lo12:str1]
	bl	uart_write
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        // -- End function
	.type	str1,@object                    // @str1
	.section	.rodata,"a",@progbits
	.globl	str1
	.p2align	4, 0x0
str1:
	.asciz	"Hello from compiler"
	.size	str1, 20

	.section	".note.GNU-stack","",@progbits
