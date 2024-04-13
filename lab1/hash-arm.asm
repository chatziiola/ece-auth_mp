// Hashing Function ARM Implementation
// Author: Lamprinos Chatziioannou
// Description: For Lab1: MP

.section .text
.align 2
.global hashfunc
.type hashfunc, %function

hashfunc:
    // @ Function prologue
    push {r4, lr}     // @ Save r4 and lr on the stack

    // @ Load constants
    ldr r3, =values
    mov r2, #0         // @ Initialize hash to 0

// @ Loop through inputString
loop:
    ldrb r4, [r0], #1 // @ Load a character from inputString and increment the pointer
    cmp r4, #0        // @ Check for null terminator
    beq end           // @ If null terminator, exit loop

    // @ Check character type
    cmp r4, #'A'
    blt check_digit	
    cmp r4, #'Z'
    bgt check_lowercase
    // @ If uppercase letter
    sub r4, r4, #'A' // @ Convert to index
    ldr r1, [r3, r4, lsl #2] // @ Load corresponding value
    add r2, r2, r1    // @ Add to hash
    b loop

check_lowercase:
    // cmp r4, #'a'
    // blt check_digit
    cmp r4, #'z'
    bgt loop
    // @ If lowercase letter
    sub r4, r4, #'a' // @ Convert to index
    ldr r1, [r3, r4, lsl #2] // @ Load corresponding value
    sub r2, r2, r1    // @ Subtract from hash
    b loop

check_digit:
    cmp r4, #'0'
    blt loop
    cmp r4, #'9'
    bgt loop
    // @ If digit
    sub r4, r4, #'0' // @ Convert to integer
    add r2, r2, r4    // @ Add to hash
    b loop

end:
    // @ Function epilogue
    pop {r4, pc}      // @ Restore r4 and return

values:
    .word 10, 42, 12, 21, 7, 5,  67, 48, 69, 2, 36, 3,  19,
          1,  14, 51, 71, 8, 26, 54, 75, 15, 6, 59, 13, 25
