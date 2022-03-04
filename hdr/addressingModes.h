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
#define IMMEDIATE_FORMAT "#%hd"
#define REGISTER_DIRECT_FORMAT "[r%hd]"


typedef enum {
	LOAD_ADRESSING_MODES(GENERATE_ENUM)
} AddressingMode_t;

/* isImmediateMode: checks if expr is a valid immediate addressing mode expression.
 * Returns 1 if it is, 0 if it isn't. */
int isImmediateMode(const char *expr, int *number);
int isDirectMode(const char *expr);
int isIndexMode(const char *expr, char destLabel[MAX_LABEL_LEN+1], int *reg);
int getAdditionalMemoryWords(AddressingMode_t mode);
int isRegisterDirectMode(const char *expr, int *reg);
int isRegister(const char *expr);

#endif
