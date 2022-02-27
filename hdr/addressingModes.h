#ifndef _ADDRESSING_MODES_H
#define _ADDRESSING_MODES_H

#include <libraries.h>

#define LOAD_ADRESSING_MODES(OPERATION)\
	OPERATION(IMMEDIATE),\
	OPERATION(DIRECT),\
	OPERATION(INDEX),\
	OPERATION(REGISTER_DIRECT)

#define MAX_REGISTER_LEN 3					/* rXX */
#define NUMBER_OF_ADDRESSING_MODES 4		/* How many addressing modes exist */
#define MAX_ADDRESSING_FORMAT_LEN \
	(MAX_LABEL_LEN + MAX_REGISTER_LEN + 2)	/* label[rXX] */

typedef enum {
	LOAD_ADRESSING_MODES(GENERATE_ENUM)
} AddressingMode_t;

#endif
