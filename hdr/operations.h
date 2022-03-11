#ifndef _OPERATIONS_H
#define _OPERATIONS_H

#include <libraries.h>
#include <encodeBits.h>
#include <addressingModes.h>

#define MIN_OPERATION_LEN 3
#define MAX_OPERATION_LEN 4
#define NUMBER_OF_OPERATIONS 16

typedef struct Operation {
	const char *opName;				/* Operation Name */
	int32_t opCode;					/* OP Code */
	int32_t functCode;				/* Funct Code */
	int numOfOperands;				/* Operands */
} Operation;

extern const Operation Operations[NUMBER_OF_OPERATIONS];

/* searchOperation: Search an operation by name. 
 * Returns the index of the operation in the const Operations array.
 * Returns FAILURE (-1) upon failure or if the operation name does not exist*/
int searchOperation(const char *opName);
int isValidOperationIndex(int operationIndex);
int getOperationMemoryWords(int operationIndex);
int isLegalOriginAddressingMode(const int operationIndex, 
								AddressingMode addressingMode);
int isLegalDestAddressingMode(const int operationIndex, 
								AddressingMode addressingMode);

#endif
