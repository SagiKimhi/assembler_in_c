; This is a file for all the errors that may appear in the first pass
;
; unknown identifier in line 4 - hello
hello r1, r2
; extraneous text in line 6
stop additionalInvalidText
; invalid line length in line 8
mov aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
; missing comma in line 10
mov r1 r1
; invalid comma in line 12
mov ,r1,r1
; invalid comma in line 14
.data 5 ,, 7
; invalid label length in line 16
thisLabelIsWayLongerThenItShouldBe: .string "hi"
; invalid label name in line 18
r14: stop
; invalid label name in line 20
mov: add #1, r1
; invalid label syntax in line 22
5Label: .data 3
; empty label tag in line 24
: .data 2
; empty label definition sentence in line 26
validLabel:
; multiple label definitions in lines 29
someLabel: mov r1, r1
someLabel: cmp r1, r2
;
; missing operands in line 32
mov 
; illegal origin addressing mode in line 34
lea #15, r2
; illegal destination addressing mode in line 36
lea someLabel, #15
; invalid data type 38
.data 48a
; missing string token " in line 40
.string hello"
; missing string token " in line 42
.string "hello
; missing string token " in line 44
.string hello
; unprintable character in line 46
.string "abcd"
; predefined non extern variable in line 48
.extern someLabel
; invalid index in line 50
mov someLabel[r1], r2
; invalid immediate operand in line 52
add #r2, r2
