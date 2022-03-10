#ifndef _ADDRESSING_MODES_H
#define _ADDRESSING_MODES_H

#include <libraries.h>

#define LOAD_ADRESSING_MODES(OPERATION)\
	OPERATION(IMMEDIATE),\
	OPERATION(DIRECT),\
	OPERATION(INDEX),\
	OPERATION(REGISTER_DIRECT)

#define MAX_REGISTER_LEN 3					/* rXX */
#define MIN_REGISTER_LEN 2					/* rX */
#define MIN_INDEX_REGISTER_NUMBER 10		/* minimum register number for index addressing mode */
#define NUMBER_OF_ADDRESSING_MODES 4		/* How many addressing modes exist */
#define IMMEDIATE_EXPRESSION_PREFIX '#'
#define REGISTER_DIRECT_EXPRESSION_PREFIX 'r'
#define IMMEDIATE_FORMAT "#%hd"
#define REGISTER_DIRECT_FORMAT "[r%hd]"


typedef enum {
	LOAD_ADRESSING_MODES(GENERATE_ENUM)
} AddressingMode_t;

int getAdditionalMemoryWords(AddressingMode_t mode);
int getAddressingMode(const char *expr);
int isRegister(const char *expr);
int scanRegister(const char *expr, int *reg);
int scanImmediateExpression(const char *expr, int *num);
int scanIndexExpression(char *expr, int *reg);

#endif
