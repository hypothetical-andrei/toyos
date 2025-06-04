.section .rodata
message:
    .asciz "Hello from prog.bin!\n"

.section .text
.global _start
_start:
    ldr x0, =0x09000000     // UARTDR
		adr x1, message
		//this does not work
    //ldr x1, =message
.loop:
    //mov x3, #'!'      // print ! before each char
    //str w3, [x0]
    ldrb w2, [x1], #1
    cbz w2, .done
    str w2, [x0]
    b .loop

.done:
    ret
