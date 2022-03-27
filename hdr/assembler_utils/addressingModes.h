#ifndef _ADDRESSING_MODES_H
#define _ADDRESSING_MODES_H
#include <libraries.h>

/* An X macro for addressing mode generation for macro
 * operations such as GENERATE_ENUM, GENERATE_STRING, etc. */
#define GENERATE_ADDRESSING_MODES(OPERATION)\
	OPERATION(IMMEDIATE),\
	OPERATION(DIRECT),\
	OPERATION(INDEX),\
	OPERATION(REGISTER_DIRECT)

#define IMMEDIATE_FORMAT		"#%hd"	/* A scanf format for an immediate expression */
#define REGISTER_DIRECT_FORMAT "r%2hd"	/* A scanf format for a direct expression */

/* An enum of all the possible addressing modes defined 
 * in the GENERATE_ADDRESSING_MODES X macro. */
typedef enum {
	GENERATE_ADDRESSING_MODES(GENERATE_ENUM)
} AddressingMode;

/* getAddressingMode: Analyzes the expression expr and returns it's
 * corrosponding addressing mode. If expr is a NULL pointer or an empty
 * string then the function will return FAILURE (-1). */
int getAddressingMode(const char *expr);

/* getAdditionalMemoryWords: Returns the number of additional memory
 * words required for representing an operand of the given addressingMode */
int getAdditionalMemoryWords(AddressingMode mode);

/* isRegister: Scans the expression expr and returns 1 if the expression 
 * represents a valid cpu register name, otherwise, returns 0. */
int isRegister(const char *expr);

/* scanRegister: Scans the expression expr and returns 1 if the expression 
 * represents a valid cpu register name, otherwise, returns 0. 
 * if expr represents a valid cpu register name, then the pointer reg is set 
 * to point to the register number expr refers to. */
int scanRegister(const char *expr, int16_t *reg);

/* scanImmediateExpression: scans an immediate expression and stores 
 * its value in the short integer pointer num.
 * Returns 1 upon success, otherwise, returns 0. */
int scanImmediateExpression(const char *expr, int16_t *num);

/* scanIndexExpression: Scans an index addressing mode expression.
 * Returns 1 if expr is indeed a valid index addressing mode expression,
 * otherwise, returns 0.
 * If expr is a valid index expression, then its index's register value 
 * is stored within the short integer pointer reg, and the character pointer
 * expr is modified such that the index delimiter is replaced by a null 
 * terminator so that expr now points to a null terminated label name. */
int scanIndexExpression(char *expr, int16_t *reg);

#endif
