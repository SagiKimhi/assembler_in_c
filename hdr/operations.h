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
 * Otherwise, upon failure, Returns FAILURE (-1). 
 *
 * @param opName A null terminated character array which holds the
 * string of the operation name to be searched for within the constant
 * array of operations. 
 * @return Returns a valid index in the constant operations array if
 * an appropriate operation was found, otherwise, FAILURE (-1) is returned. */
int searchOperation(const char *opName);

/* isValidOperationIndex: checks whether or not the argument operationIndex is 
 * a valid operationIndex in the constant Operations array. 
 * Returns 1 if it is, otherwise, 0 is returned. 
 *
 * @param operationIndex An integer value to be checked as a
 * valid index in the constant Operations array. 
 * @return Returns 1 if the provided index is a valid index, or 0 if it isnt. */
int isValidOperationIndex(int operationIndex);

/* getOperationMemoryWords: Returns the amount of memory words required to solely represent
 * the operation from the Operations array at the provided operationIndex in machine code,
 * without taking into account any additional operands, addressing modes, etc. 
 * If the provided operationIndex is not a valid index, 0 is returned. 
 *
 * @param operationIndex An integer value which was previously returned by
 * the searchOperation function that represents an index of the operation
 * in the constant Operations array whose number of required memory words
 * are to be returned by this function.
 * @return Returns the amount of a memory words required to solely represent
 * the operation specified by operationIndex in machine code as a positive integer. 
 * if operationIndex is not a valid index then 0 will be returned. */
int getOperationMemoryWords(int operationIndex);

/* isLegalOriginAddressingMode: Returns 1 if addressingMode is a valid origin 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. 
 *
 * @param operationIndex An index previously returned by searchOperation which represents
 * a certain operation from the operations defined in the constant Operations array. 
 * @param addressingMode The AddressingMode to be checked for being a legal origin
 * addressing mode for the specified operation. 
 * @return Returns 1 if addressingMode is a valid origin AddressingMode for
 * the specified operation, otherwise, 0 is returned. */
int isLegalOriginAddressingMode(const int operationIndex, AddressingMode addressingMode);

/* isLegalDestAddressingMode: Returns 1 if addressingMode is a valid destination 
 * AddressingMode for the operation located at the provided operationIndex in 
 * the constant Operations array, otherwise, returns 0. 
 *
 * @param operationIndex An index previously returned by searchOperation which represents
 * a certain operation from the operations defined in the constant Operations array. 
 * @param addressingMode The AddressingMode to be checked for being a legal destination
 * addressing mode for the specified operation. 
 * @return Returns 1 if addressingMode is a valid destination AddressingMode for
 * the specified operation, otherwise, 0 is returned. */
int isLegalDestAddressingMode(const int operationIndex, AddressingMode addressingMode);

#endif
