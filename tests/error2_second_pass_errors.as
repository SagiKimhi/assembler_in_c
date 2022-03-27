; This file contains errors which pop up in the second pass
; The first part consists of a valid test provided in the maman instructions up to line 24
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
 lea STR, r6
 inc r6
 mov r3, W
 sub r1, r4
 bne END
 cmp val1, #-6
 bne END[r15]
 dec K
.entry MAIN
 sub LOOP[r10] ,r14
END: stop
STR: .string "abcd"
LIST: .data 6, -9
 .data -100
.entry K
K: .data 31
.extern val1
; ********** The 2nd errors start here ********* ;
; Origin operand is undefined label in line 26
mov loop, r2
; Destination operand is undefined label in line 28
mov r2, loop
; entry is undefined label in line 30
.entry List
; entry label was already declared as extern in line 32
.entry W
