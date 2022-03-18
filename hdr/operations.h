#ifndef _OPERATIONS_H
#define _OPERATIONS_H
#include <libraries.h>
#include <encodeBits.h>
#include <addressingModes.h>

/* ----------------------------------------------------------------	*
 *			    Constants, Defines, Typedefs, etc.  			    *
 * ----------------------------------------------------------------	*/
#define MIN_OPERATION_LEN 3
#define MAX_OPERATION_LEN 4
#define NUMBER_OF_OPERATIONS 16

typedef struct Operation {
	const char *opName;				/* Operation Name */
	int32_t opCode;					/* OP Code */
	int32_t functCode;				/* Funct Code */
	int numOfOperands;				/* Operands */
} Operation;

/* A constant Operation array specifying all the possible operations
 * and their corrosponding name, op code, funct code, and no. of operands */
extern const Operation Operations[NUMBER_OF_OPERATIONS];
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* searchOperation: Searches an operation by name, if the operation is 
 * found in the constant Operation array of operations, the function
 * returns the index of that operation within the array. 
 * Otherwise, upon failure, Returns FAILURE (-1). */
int searchOperation(const char *opName);

/* isValidOperationIndex: checks whether or not the argument operationIndex is 
 * a valid operationIndex in the constant Operations array. 
 * Returns 1 if it is, otherwise, 0 is returned. */
int isValidOperationIndex(int operationIndex);

/* getOperationMemoryWords: Returns the amount of memory words required to solely represent
 * the operation from the Operations array at the provided operationIndex in machine code,
 * without taking into account any additional operands, addressing modes, etc. 
 * If the provided operationIndex is not a valid index, 0 is returned. */
int getOperationMemoryWords(int operationIndex);

/* isLegalOriginAddressingMode: Returns 1 if addressingMode is a valid origin 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. */
int isLegalOriginAddressingMode(const int operationIndex, AddressingMode addressingMode);

/* isLegalDestAddressingMode: Returns 1 if addressingMode is a valid destination 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. */
int isLegalDestAddressingMode(const int operationIndex, AddressingMode addressingMode);

#endif
