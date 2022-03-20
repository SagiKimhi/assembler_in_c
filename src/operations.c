#include <operations.h>

/* ----------------------------------------------------------------	*
 *			    Constants, Defines, Typedefs, etc.  			    *
 * ----------------------------------------------------------------	*/
/* An X macro for operations generation to be generated by 
 * macro operations such as GENERATE_ENUM, GENERATE_STRING, etc. 
 *
 * This X macro was chosen to be kept within the source file instead
 * of the header file in order to prevent the ability of users to make
 * changes which may affect the functionality of the functions within this
 * file and cause unexpected behaviours mostly related to invalid values
 * being returned from the functions due to the changes. */
#define GENERATE_OPERATIONS(OPERATION)\
	OPERATION(mov),\
	OPERATION(cmp),\
	OPERATION(add),\
	OPERATION(sub),\
	OPERATION(lea),\
	OPERATION(clr),\
	OPERATION(not),\
	OPERATION(inc),\
	OPERATION(dec),\
	OPERATION(jmp),\
	OPERATION(bne),\
	OPERATION(jsr),\
	OPERATION(red),\
	OPERATION(prn),\
	OPERATION(rts),\
	OPERATION(stop)

/* An enum of all of the possible operations defined 
 * in the GENERATE_LABEL_TYPES X macro. */
enum OperationEnums {
	GENERATE_OPERATIONS(GENERATE_ENUM)
};

/* A constant string array which contains strings
 * of all possible label types. The string is generated
 * using the GENERATE_OPERATIONS X Macro along with the
 * GENERATE_STRING operation and therefore names are acceissible
 * using enum operation indices, i.e OperationNames[mov] == "mov". */
const char OperationNames[NUMBER_OF_OPERATIONS][MAX_OPERATION_LEN+1] = {
	GENERATE_OPERATIONS(GENERATE_STRING)
};

/* A constant Operation array specifying all the possible operations
 * and their corrosponding name, op code, funct code, and no. of operands */
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
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* searchOperation: Searches an operation by name, if the operation is 
 * found in the constant Operation array of operations, the function
 * returns the index of that operation within the array. 
 * Otherwise, upon failure, Returns FAILURE (-1). */
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

/* isValidOperationIndex: checks whether or not the argument operationIndex is 
 * a valid operationIndex in the constant Operations array. 
 * Returns 1 if it is, otherwise, 0 is returned. */
int isValidOperationIndex(int operationIndex)
{
	return (operationIndex>=0 && operationIndex<=NUMBER_OF_OPERATIONS);
}

/* getOperationMemoryWords: Returns the amount of memory words required to solely represent
 * the operation from the Operations array at the provided operationIndex in machine code,
 * without taking into account any additional operands, addressing modes, etc. 
 * If the provided operationIndex is not a valid index, 0 is returned. */
int getOperationMemoryWords(int operationIndex)
{
	if (!isValidOperationIndex(operationIndex))
		return 0;

	return ((!Operations[operationIndex].numOfOperands) ? 1: 2);
}

/* isLegalOriginAddressingMode: Returns 1 if addressingMode is a valid origin 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. */
int isLegalOriginAddressingMode(const int operationIndex, AddressingMode addressingMode)
{
	switch (operationIndex) {
		/* Every addressing mode is legal */
		case mov: case cmp: 
		case add: case sub:
			return 1;

		/* only direct and index are legal */
		case lea:
			switch (addressingMode) {
				case DIRECT: case INDEX:
					return 1;
				default: 
					return 0;
			}

		default:
			return 0;
	}
}

/* isLegalDestAddressingMode: Returns 1 if addressingMode is a valid destination 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. */
int isLegalDestAddressingMode(const int operationIndex, AddressingMode addressingMode)
{
	switch (operationIndex) {
		/* Every addressing mode is legal */
		case cmp: case prn:
			return 1;

		/* only direct, index, and register direct are legal */
		case mov: case add: case sub: case lea: case clr: 
		case not: case inc: case dec: case red:
			switch (addressingMode) {
				case DIRECT: case INDEX: 
				case REGISTER_DIRECT:
					return 1;

				default:
					return 0; 
			}

		/* only direct and index are legal */
		case jmp: case bne: case jsr:
			switch (addressingMode) {
				case DIRECT: case INDEX:
					return 1;

				default: 
					return 0;
			}

		default:
			return 0;
	}
}
