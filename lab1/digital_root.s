// Function that calculates the digital root of an integer
// Author: Georgios Selivanof
// Description: For Lab1: MP

.global digital_root
.p2align 2

.section .text
.type digital_root, %function


digital_root:
	.fnstart
	
	push {lr}
	// Using registers:
	// r0: Input Argument, Return Value
	// r1: Reciprocal
	// r2: Quotient of division by 9
	ldr r1,  reciprocal     // Loading the reciprocal into r1
	mov r2, #0              // Initialize r2 (result) to 0
	
	cmp r0, #0             
	beq end                 // If the input is 0, return 0
		
	bgt div9
	rsb r0, r0, #0          // Convert negative number to positive
div9:
	umull r3, r2, r1, r0    // Multiply with the reciprocal
	lsr r2, r2, #1          // Left shift on the lower 32 bits, the result requals r0 / 9
	
	                        // Find the Remainder
	add r2, r2, r2, lsl #3  // Multiply the quotient by 9 (Alternative intruction mul r2, r2, #9)
	sub r0, r0, r2          // Remove the quotient * 9 from the original number, Get the remainder
	
	cmp r0, #0              // If the remainder is 0, the digital root is 9
	bne end
	mov r0, #9              // Change the result to 9
	
end:
	pop {pc}
	.fnend
	
reciprocal:
	.word 954437177         // = (2^33) / 9
