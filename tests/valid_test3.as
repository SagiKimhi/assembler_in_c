; file originalMamanTest.as
data0: .data 0
data1: .data 1
data2: .data 2
data3: .data 3
data4: .data 4
data5: .data 5
data6: .data 6
data7: .data 7
data8: .data 8
data9: .data 9
data10: .data 10
data11: .data 11
string0: .string "0"
string1: .string "1"
string2: .string "2"
string3: .string "3"
string4: .string "4"
string5: .string "5"
string6: .string "6"
string7: .string "7"
string8: .string "8"
string9: .string "9"
string10: .string "10"
.entry LIST
.extern W
MAIN: add r3, LIST
LOOP: prn #48
 lea STR, r6
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
entry0: .entry data0
entry1: .entry data1
entry2: .entry data2
entry3: .entry data3
entry4: .entry data4
entry5: .entry data5
entry6: .entry data6
entry7: .entry data7
entry8: .entry data8
entry9: .entry data9
entry10: .entry data10
entry11: .entry string0
entry12: .entry string1
entry13: .entry string2
entry14: .entry string3
entry15: .entry string4
entry16: .entry string5
entry17: .entry string6
entry18: .entry string7
entry19: .entry string8
entry20: .entry string9
entry21: .entry string10
extern0: .extern string11
extern1: .extern string12
extern2: .extern string13
extern3: .extern string14
extern4: .extern string15
extern5: .extern string16
extern6: .extern string17
extern7: .extern string18
extern8: .extern string19
extern9: .extern string20
extern10: .extern string21
extern11: .extern string22
extern12: .extern string23
extern13: .extern string24
extern14: .extern string25
extern15: .extern string26
extern16: .extern string27
extern17: .extern string28
extern18: .extern string29
extern19: .extern string30
extern20: .extern string31
extern21: .extern string32
extern22: .extern string33
extern23: .extern string34
extern24: .extern string35
extern25: .extern string36
extern26: .extern string37
extern27: .extern string38
extern28: .extern string39
extern29: .extern string40
extern30: .extern string41
extern31: .extern string42
extern32: .extern string43
extern33: .extern string44
extern34: .extern string45
extern35: .extern string46
extern36: .extern string47
extern37: .extern string48

cmp data0,string0
cmp data1,string1
cmp data2,string2
cmp data3,string3
cmp data4,string4
cmp data5,string5
cmp data6,string6
cmp data7,string7
cmp data8,string8
cmp data9,string9
cmp data10,string10
;cmp data11,string11
;cmp data12,string12
;cmp data13,string13
;cmp data14,string14
;cmp data15,string15
;cmp data16,string16
;cmp data17,string17
;cmp data18,string18
;cmp data19,string19
;cmp data20,string20
;cmp data21,string21
add  data0[r10] , data0
add  data1[r10] , data1
add  data2[r10] , data2
add  data3[r10] , data3
add  data4[r10] , data4
add  data5[r10] , data5
add  data6[r10] , data6
add  data7[r10] , data7
add  data8[r10] , data8
add  data9[r10] , data9
add  data10[r10] , data10
add  string0[r10] , data0
add  string1[r10] , data1
add  string2[r10] , data2
add  string3[r10] , data3
add  string4[r10] , data4
add  string5[r10] , data5
add  string6[r10] , data6
add  string7[r10] , data7
add  string8[r10] , data8
add  string9[r10] , data9
add  string10[r10] , data10
;  add  string11[r10] , data11
;  add  string12[r10] , data12
;  add  string13[r10] , data13
;  add  string14[r10] , data14
;  add  string15[r10] , data15
;  add  string16[r10] , data16
;  add  string17[r10] , data17
;  add  string18[r10] , data18
;  add  string19[r10] , data19
;  add  string20[r10] , data20
;  add  string21[r10] , data21
;  add  string22[r10] , data22
;  add  string23[r10] , data23
;  add  string24[r10] , data24
;  add  string25[r10] , data25
;  add  string26[r10] , data26
;  add  string27[r10] , data27
;  add  string28[r10] , data28
;  add  string29[r10] , data29
;  add  string30[r10] , data30
;  add  string31[r10] , data31
add  string11[r10] , string11
add  string12[r10] , string12
add  string13[r10] , string13
add  string14[r10] , string14
add  string15[r10] , string15
add  string16[r10] , string16
add  string17[r10] , string17
add  string18[r10] , string18
add  string19[r10] , string19
add  string20[r10] , string20
add  string21[r10] , string21
add  string22[r10] , string22
add  string23[r10] , string23
add  string24[r10] , string24
add  string25[r10] , string25
add  string26[r10] , string26
add  string27[r10] , string27
add  string28[r10] , string28
add  string29[r10] , string29
add  string30[r10] , string30
add  string31[r10] , string31
add  string32[r10] , string32
