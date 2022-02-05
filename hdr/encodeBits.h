#ifndef _ENCODE_BITS_H
#define _ENCODE_BITS_H

#define NONE 0
#define BIT_0 0x1
#define BIT_1 0x2
#define BIT_2 0x4
#define BIT_3 0x8
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80
#define BIT_8 0x100
#define BIT_9 0x200
#define BIT_10 0x400
#define BIT_11 0x800
#define BIT_12 0x1000
#define BIT_13 0x2000
#define BIT_14 0x4000
#define BIT_15 0x8000
#define BIT_16 0x10000
#define BIT_17 0x20000
#define BIT_18 0x40000
#define BIT_19 0x80000

/* These should probably be defined as functions
 * instead of macros (Note to self) */
#define encodeOriginAddressMode(X) (X)
#define encodeOriginRegister(X) ((X)<<2)
#define encodeDestAddressMode(X) ((X)<<6)
#define encodeDestRegister(X) ((X)<<8)
#define encodeFunct(X) ((X)<<12)
#define encodeInstructionCode(X) ((X)<<16)
#define enccodePSW(X) ((X) ? BIT_19: NONE)

#endif
