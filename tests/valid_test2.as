; file test1.as
macro m1
 inc r6
 mov r3, W
endm
macro instruction1
 inc r6
 mov r3, W
endm
macro instruction2
 inc r6
 mov r3, W
endm
macro instruction3
 inc r6
 mov r3, W
endm
macro instruction4
 inc r6
 mov r3, W
endm
macro instruction5
 inc r6
 mov r3, W
endm
macro instruction6
 inc r6
 mov r3, W
endm
macro instruction6
 inc r6
 mov r3, W
endm
macro instruction7
 inc r6
 mov r3, W
endm
macro instruction8
 inc r6
 mov r3, W
endm
macro instruction9
 inc r6
 mov r3, W
endm
macro instruction10
 inc r6
 mov r3, W
endm
macro instruction11
 inc r6
 mov r3, W
endm
macro instruction12
 inc r6
 mov r3, W
endm
macro instruction13
 inc r6
 mov r3, W
endm
macro instruction14
 inc r6
 mov r3, W
endm
macro instruction15
 inc r6
 mov r3, W
endm
macro instruction16
 inc r6
 mov r3, W
endm
macro instruction17
 inc r6
 mov r3, W
endm
macro instruction18
 inc r6
 mov r3, W
endm
macro instruction19
 inc r6
 mov r3, W
endm
macro instruction20
 inc r6
 mov r3, W
endm
macro data1
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data2
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data3
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data4
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data5
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data6
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data7
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data8
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data9
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data10
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data11
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data12
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data13
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data14
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data15
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data16
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data17
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data18
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data19
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
macro data20
 .data 1, 2, 3, 4,5,6,7,8
 .string "This Is a test"
endm
instruction1
instruction2
instruction3
instruction4
instruction5
instruction6
instruction7
instruction8
instruction9
instruction10
instruction11
instruction12
instruction13
instruction14
instruction15
instruction16
instruction17
instruction18
instruction19
instruction20
data1
data2
data3
data4
data5
data6
data7
data8
data9
data10
data11
data12
data13
data14
data15
data16
data17
data18
data19
data20

; From here on this is like the original test
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
 lea STR, r6
 m1
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

