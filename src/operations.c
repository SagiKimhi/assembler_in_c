#include <operations.h>

enum OperationEnums {
	LOAD_OPERATIONS(GENERATE_ENUM)
};

const char OperationNames[NUMBER_OF_OPERATIONS][MAX_OPERATION_LEN+1] = {
	LOAD_OPERATIONS(GENERATE_STRING)
};

const Operation Operations[NUMBER_OF_OPERATIONS] = {
				/* 2 Operand operations: */
	{	/* mov */
		OperationNames[mov],	/* Operation Name */
		BIT_0,					/* OP Code */
		NONE,					/* Funct Code */
		2						/* Operands */
	},
	{	/* cmp */
		OperationNames[cmp],	/* Operation Name */
		BIT_1,					/* OP Code */
		NONE,					/* Funct Code */
		2						/* Operands */
	},
	{	/* add */
		OperationNames[add],	/* Operation Name */
		BIT_2,					/* OP Code */
		encodeFunct(10),		/* Funct Code */
		2						/* Operands */
	},
	{	/* sub */
		OperationNames[sub],	/* Operation Name */
		BIT_2,              	/* OP Code */
		encodeFunct(11),    	/* Funct Code */
		2                   	/* Operands */
	},
	{	/* lea */
		OperationNames[lea],	/* Operation Name */
		BIT_4,              	/* OP Code */
		NONE,               	/* Funct Code */
		2                   	/* Operands */
	},
				/* 1 Operand operations: */
	{	/* clr */
		OperationNames[clr],	/* Operation Name */
		BIT_5,              	/* OP Code */
		encodeFunct(10),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* not */
		OperationNames[not],	/* Operation Name */
		BIT_5,              	/* OP Code */
		encodeFunct(11),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* inc */
		OperationNames[inc],	/* Operation Name */
		BIT_5,              	/* OP Code */
		encodeFunct(12),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* dec */
		OperationNames[dec],	/* Operation Name */
		BIT_5,              	/* OP Code */
		encodeFunct(13),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* jmp */
		OperationNames[jmp],	/* Operation Name */
		BIT_9,              	/* OP Code */
		encodeFunct(10),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* bne */
		OperationNames[bne],	/* Operation Name */
		BIT_9,              	/* OP Code */
		encodeFunct(11),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* jsr */
		OperationNames[jsr],	/* Operation Name */
		BIT_9,              	/* OP Code */
		encodeFunct(12),    	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* red */
		OperationNames[red],	/* Operation Name */
		BIT_12,             	/* OP Code */
		NONE,               	/* Funct Code */
		1                   	/* Operands */
	},
	{	/* prn */
		OperationNames[prn],	/* Operation Name */
		BIT_13,             	/* OP Code */
		NONE,               	/* Funct Code */
		1                   	/* Operands */
	},
				/* 0 Operands operations: */
	{	/* rts */
		OperationNames[rts],	/* Operation Name */
		BIT_14,             	/* OP Code */
		NONE,               	/* Funct Code */
		0                   	/* Operands */
	},
	{	/* stop */
		OperationNames[stop],	/* Operation Name */
		BIT_15,              	/* OP Code */
		NONE,                	/* Funct Code */
		0                    	/* Operands */
	}
};

/* searchOperation: Search an operation by name. 
 * Returns the index of the operation in the const Operations array.
 * Returns FAILURE (-1) upon failure or if the operation name does not exist*/
int searchOperation(const char *opName)
{
	int i, len;

	if (!opName)
		return FAILURE;

	if ((len=strlen(opName))<MIN_OPERATION_LEN || len>MAX_OPERATION_LEN)
		return FAILURE;

	for (i=0; i<NUMBER_OF_OPERATIONS; i++)
		if (!strcmp(Operations[i].opName, opName))
				return i;

	return FAILURE;
}

int32_t getOpCode(const char *opName)
{
	int i;

	if ((i=searchOperation(opName))==FAILURE)
		return FAILURE;

	return Operations[i].opCode;
}

int32_t getFunctCode(const char *opName)
{
	int i;

	if ((i=searchOperation(opName))==FAILURE)
		return FAILURE;

	return Operations[i].functCode;
}

int getNumOfOperands(const char *opName)
{
	int i;

	if ((i=searchOperation(opName))==FAILURE)
		return FAILURE;

	return Operations[i].numOfOperands;
}