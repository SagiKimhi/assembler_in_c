#ifndef _TEMP_H
#define _TEMP_H
#include <libraries.h>

#define MIN_OPERANDS 0
#define MAX_OPERANDS 2

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

#define LOAD_ADRESSING_MODES(OPERATION)\
	OPERATION(IMMEDIATE),\
	OPERATION(DIRECT),\
	OPERATION(INDEX),\
	OPERATION(REG_DIRECT)

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

enum Operations {
	LOAD_OPERATIONS(GENERATE_ENUM)
};

typedef enum OperationsCodes {
	op_mov = BIT_0,
	op_cmp = BIT_1,
	op_add = BIT_2,
	op_sub = BIT_2,
	op_lea = BIT_4,
	op_clr = BIT_5,
	op_not = BIT_5,
	op_inc = BIT_5,
	op_dec = BIT_5,
	op_jmp = BIT_9,
	op_bne = BIT_9,
	op_jsr = BIT_9,
	op_red = BIT_12,
	op_prn = BIT_13,
	op_rts = BIT_14,
	op_stop =BIT_15 
} OpCode;

typedef enum FunctCodes {
	none = NONE,
	funct_add = encodeFunct(10),
	funct_clr = encodeFunct(10),
	funct_jmp = encodeFunct(10),
	funct_sub = encodeFunct(11),
	funct_not = encodeFunct(11),
	funct_bne = encodeFunct(11),
	funct_inc = encodeFunct(12),
	funct_jsr = encodeFunct(12),
	funct_dec = encodeFunct(13)
} Funct;

enum AddressingModes_enum {
	LOAD_ADRESSING_MODES(GENERATE_ENUM)
};

typedef struct _AddressingMode {
	int8_t additionalWords;	/* additional words required for the addressing mode */
	uint8_t originBinRep;	/* origin addressing mode field binary representation */
	uint8_t destBinRep;		/* destination addressing mode field binary representation */
} AddressingMode;

extern const AddressingMode addressingMode[];

typedef struct _Operation {
	OpCode opCode;			/* the operation's op code */
	Funct funct;			/* the operation's funct code (none if there's none) */
	int8_t operands;		/* the number of operands the operation requires */
} Operation;

extern const Operation operation[];
extern const char operationNames[][MAX_OPERATION_LEN+1];

#endif
