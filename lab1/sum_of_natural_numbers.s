// Function that calculates the sum of natural numbers
// Author: Georgios Selivanof
// Description: For Lab1: MP

.global sum_of_natural_numbers
.p2align 2

.section .text
.type sum_of_natural_numbers, %function


sum_of_natural_numbers:
	push {lr}
	// Using registers:
	// r0: Input Argument, Return Value
	// r1: Sum/Result
	mov r1, #0          // Initialize r1 (result) to 0
loop:
	cmp r0, #0          // If the current input is 0, return
	ble end
	add r1, r1, r0      // Add the current input to the sum
	sub r0, r0, #1      // Remove 1 from r0 (input)
	b loop              // Repeat
end:
	mov r0, r1          // Move r1 (result) to r0 (return)
	pop {pc}
