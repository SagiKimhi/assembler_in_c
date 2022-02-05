#include <tempName.h>

const AddressingMode addressingMode[] = {
	/* Immediate addressing mode specs: */
	{
		1,									/* additionalWords */
		encodeOriginAddressMode(IMMEDIATE),	/* originBinRep */
		encodeDestAddressMode(IMMEDIATE)	/* destBinRep */
	},

	/* Direct addressing mode specs: */
	{
		2,									/* additionalWords */
		encodeOriginAddressMode(DIRECT),	/* originBinRep */
		encodeDestAddressMode(DIRECT)		/* destBinRep */
	},

	/* Index addressing mode specs: */
	{
		2,											/* additionalWords */
		encodeOriginAddressMode(INDEX),				/* originBinRep */
		encodeDestAddressMode(INDEX)				/* destBinRep */
	},

	/* Register direct addressing mode specs: */
	{
		0,									/* additionalWords */
		encodeOriginAddressMode(REG_DIRECT),/* originBinRep */
		encodeDestAddressMode(REG_DIRECT)	/* destBinRep */
	}
};

/* Since an enum is also defined, it allows accessing
 * operations in the array by name, for example:
 * operationNames[sub] will return "sub" */
const char operationNames[][MAX_OPERATION_LEN+1] = {
	LOAD_OPERATIONS(GENERATE_STRING)
};

const Operation operation[] = {

	/* {op code, funct, #operands} */

	{op_mov, none, 2},		/* mov */
	{op_cmp, none, 2},		/* cmp */
	{op_add, funct_add, 2},	/* add */
	{op_sub, funct_sub, 2},	/* sub */
	{op_lea, none, 2},		/* lea */
	{op_clr, funct_clr, 1},	/* clr */
	{op_not, funct_not, 1},	/* not */
	{op_inc, funct_inc, 1},	/* inc */
	{op_dec, funct_dec, 1},	/* dec */
	{op_jmp, funct_jmp, 1},	/* jmp */
	{op_bne, funct_bne, 1},	/* bne */
	{op_jsr, funct_jsr, 1},	/* jsr */
	{op_red, none, 1},		/* red */
	{op_prn, none, 1},		/* prn */
	{op_rts, none, 0},		/* rts */
	{op_stop, none, 0}		/* stop */

};
