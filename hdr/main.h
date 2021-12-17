#ifndef _MAIN_H
#define _MAIN_H

/* Standard Lib Headers: */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Our Headers: */
#include <sizes.h>

/* Defines and Macros: */
#ifndef __X_MACROS__
#define __X_MACROS__

#ifdef __FOREACH_OPERATION_CELL_INDEX__
#undef __FOREACH_OPERATION_CELL_INDEX__
#endif
#define __FOREACH_OPERATION_CELL_INDEX__
#define FOREACH_OPERATION_CELL_INDEX(OPERATION) \
	OPERATION(dAddrs0) \
	OPERATION(dAddrs1) \
	OPERATION(dRegister0) \
	OPERATION(dRegister1) \
	OPERATION(dRegister2) \
	OPERATION(dRegister3) \
	OPERATION(oAddrs0) \
	OPERATION(oAddrs1) \
	OPERATION(oRegister0) \
	OPERATION(oRegister1) \
	OPERATION(oRegister2) \
	OPERATION(oRegister3) \
	OPERATION(funct0) \
	OPERATION(funct1) \
	OPERATION(funct2) \
	OPERATION(funct3) \
	OPERATION(absolute) \
	OPERATION(relocatable) \
	OPERATION(external) \
	OPERATION(_0_)

#ifdef __FOREACH_HEX_VALUE__
#undef __FOREACH_HEX_VALUE__
#endif
#define __FOREACH_HEX_VALUE__
#define FOREACH_HEX_VALUE(OPERATION) \
	OPERATION(_0) \
	OPERATION(_1) \
	OPERATION(_2) \
	OPERATION(_3) \
	OPERATION(_4) \
	OPERATION(_5) \
	OPERATION(_6) \
	OPERATION(_7) \
	OPERATION(_8) \
	OPERATION(_9) \
	OPERATION(_A) \
	OPERATION(_B) \
	OPERATION(_C) \
	OPERATION(_D) \
	OPERATION(_E) \
	OPERATION(_F) 

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#endif

/* Constants, Typedefs, etc. : */
enum OP_INSTRUCTIONS_ENUM {
	FOREACH_OPERATION_CELL_INDEX(GENERATE_ENUM)
};

const char *OP_INSTRUCTIONS_STRING[] = {
	FOREACH_OPERATION_CELL_INDEX(GENERATE_STRING)
};

enum HEX_VALUES_ENUM {
	FOREACH_HEX_VALUE(GENERATE_ENUM)
};

const char *HEX_VALUES_STRING[] = {
	FOREACH_HEX_VALUE(GENERATE_STRING)
};

enum opcodes {
	_mov = 0,
	_cmp = 1,			/* The opcode field represents 16 	*/
	_add = 2,			/* bits, where only one bit may be	*/
	_sub = 2,			/* turned on  - in accordance with	*/
	_lea = 4,			/* the code of the operation. 		*/
	_clr = 5,
	_not = 5,
	_inc = 5,
	_dec = 5,
	_jmp = 9,
	_bne = 9,
	_jsr = 9,
	_red = 12,
	_prn = 13,
	_rts = 14,
	_stop = 15
};

enum functs {
	_fadd = 0,		/* The funct field is only used	for */
	_fsub = 1,		/* operations which share a single 	*/
	_fclr = 0,		/* opcode between them. 			*/
	_fnot = 1,		/* This field holds a unique value 	*/
	_finc = 2,		/* for every operation in one of 	*/
	_fdec = 3,		/* these groups. 					*/
	_fjmp = 0,		/* If an operation does not share	*/
	_fbne = 1,		/* an opcode with other	operations	*/
	_fjsr = 2,		/* then funct will be set to 0. 	*/
};

enum addressingModes {
	IMMEDIATE,
	DIRECT,
	INDEX,
	REGISTER_DIRECT
};

typedef union {
	uint8_t memoryCell[MEMSIZE];

	struct no_op_instruction{
		uint8_t opcode[OPCODE];
		uint8_t instructCode[INSTRUCTION_CODE];
	};

	struct op_instruct{
		uint8_t dAddrssMode[D_ADDRSS_MODE];
		uint8_t dRegister[D_REGISTER];
		uint8_t oAddrssMode[O_ADDRSS_MODE];
		uint8_t oRegister[O_REGISTER];
		uint8_t funct[FUNCT];
		uint8_t instructCode[INSTRUCTION_CODE];
	};
} Instruction;

/* enum _toggleBits {
	RESET = 0x0u,
	_0 = 0x1u,
	_1 = 0x2u,
	_2 = 0x4u,
	_3 = 0x8u,
	_4 = 0x10u,
	_5 = 0x20u,
	_6 = 0x40u,
	_7 = 0x80u,
	_8 = 0x100u,
	_9 = 0x200u,
	_10 = 0x400u,
	_11 = 0x800u,
	_12 = 0x1000u,
	_13 = 0x2000u,
	_14 = 0x4000u,
	_15 = 0x8000u,
	_16 = 0x10000u,
	_17 = 0x20000u,
	_18 = 0x40000u,
	_19 = 0x80000u
}; */
#endif