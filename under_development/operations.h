#ifndef _OPERATIONS_H
#define _OPERATIONS_H

#include <libraries.h>
#include <encodeBits.h>

#define LOAD_OPERATIONS(OPERATION)\
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

#define MIN_OPERATION_LEN 3
#define MAX_OPERATION_LEN 4
#define NUMBER_OF_OPERATIONS 16

typedef struct _Operation {
	char opName[MAX_OPERATION_LEN+1];	/* Operation Name */
	int32_t opCode;						/* OP Code */
	int32_t functCode;					/* Funct Code */
	int numOfOperands;					/* Operands */
} Operation;

#endif
