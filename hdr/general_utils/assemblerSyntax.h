#ifndef _ASSEMBLER_SYNTAX_H
#define _ASSEMBLER_SYNTAX_H
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------------------------	*
 *					FILE RELATED SYNTAX DEFINES  			    *
 * ----------------------------------------------------------------	*/
#define FILE_EXTENSION_TOKEN		'.'
#define SOURCE_FILE_EXTENSION		".as"
#define PREPROCESSED_FILE_EXTENSION ".am"
#define OBJECT_FILE_EXTENSION		".ob"
#define ENTRY_FILE_EXTENSION		".ent"
#define EXTERN_FILE_EXTENSION		".ext"
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *				  ASSEMBLY LANGUAGE TOKENS DEFINES					*
 * ----------------------------------------------------------------	*/
#define COMMENT_PREFIX						';'
#define OPERAND_SEPERATOR					','
#define OPERAND_SEPERATORS					", \t\n"
#define DIRECTIVE_TOKEN_PREFIX				'.'
#define LABEL_DEFINITION_SUFFIX				':'
#define IMMEDIATE_EXPRESSION_PREFIX			'#'
#define REGISTER_DIRECT_EXPRESSION_PREFIX	'r'
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *				  SIZE AND LENGTH LIMITATION DEFINES				*
 * ----------------------------------------------------------------	*/
/* Lengths */
#define MAX_LINE_LEN				80
#define MAX_LABEL_LEN				31
#define MAX_MACRO_LEN				31
#define MIN_OPERATION_LEN			3
#define MAX_OPERATION_LEN			4
#define MAX_FILE_EXTENSION_LEN		4

/* Memory word related defines */
#define MEMSIZE						8192
#define ADDRESS_BASE				16
#define MEM_CELL_SIZE				20
#define FIRST_MEMORY_ADDRESS		100
#define MAX_OPERATION_WORDS			2
#define MAX_ADDITIONAL_WORDS		4

/* MIN/MAX Limitations */
#define MIN_REGISTER_NUMBER			0
#define MIN_INDEX_REGISTER_NUMBER	10
#define MAX_REGISTER_NUMBER			15
#define NUMBER_OF_OPERATIONS		16
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *				  SAVED ASSEMBLY LANGAUGE KEYWORDS					*
 * ----------------------------------------------------------------	*/
#define START_OF_MACRO_DEFINITION	"macro"
#define END_OF_MACRO_DEFINITION		"endm"
#define START_DATA_DEFINITION		"data"
#define START_STRING_DEFINITION 	"string"
#define START_ENTRY_DEFINITION		"entry"
#define START_EXTERN_DEFINITION 	"extern"
/* isSavedKeyword: Checks if expression is a saved assembler 
 * syntax keyword. Returns 1 if it is or 0 if it isn't. 
 *
 * @param expr A null terminated character array which holds the
 * expression to be checked for being a saved syntax keyword.
 * @return Returns 1 if expr is a predefined assembler syntax keyword
 * or 0 if it isn't. */
extern int isSavedKeyword(const char *expr);
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *				BITS AND BIT FIELD ENCODING UTILITY MACROS			*
 * ----------------------------------------------------------------	*/
/* Bits */
#define NONE				0x0
#define BIT_0				0x1
#define BIT_1				0x2
#define BIT_2				0x4
#define BIT_3				0x8
#define BIT_4				0x10
#define BIT_5				0x20
#define BIT_6				0x40
#define BIT_7				0x80
#define BIT_8				0x100
#define BIT_9				0x200
#define BIT_10				0x400
#define BIT_11 				0x800
#define BIT_12 				0x1000
#define BIT_13 				0x2000
#define BIT_14 				0x4000
#define BIT_15 				0x8000
#define BIT_16 				0x10000
#define BIT_17 				0x20000
#define BIT_18				0x40000
#define BIT_19				0x80000
#define EXTERNAL_CODE		BIT_16
#define RELOCATABLE_CODE	BIT_17
#define ABSOLUTE_CODE		BIT_18

/* Bit encoding macros for specific fields in an operation's 2nd memory word */
#define encodeDestAddressMode(X)	(X)					/* Bits [1:0] */
#define encodeDestRegister(X)		((X)<<2)			/* Bits [5:2] */
#define encodeOriginAddressMode(X)	((X)<<6)			/* Bits [7:6] */
#define encodeOriginRegister(X)		((X)<<8)			/* Bits [11:8] */
#define encodeFunct(X)				((X)<<12)			/* Bits [15:12] */
#define encodeInstructionCode(X)	((X)<<16)			/* Bits [18:16] */
#define encodePSW(X)				((X) ? BIT_19: NONE)/* Bits [19:19] */
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *				OTHER GENERAL ASSEMBLER UTILITY DEFINES				*
 * ----------------------------------------------------------------	*/
#define FAILURE						-1
#define GENERATE_ENUM(ENUM)			ENUM
#define GENERATE_STRING(STRING)		#STRING
/* ----------------------------------------------------------------	*/

#endif
