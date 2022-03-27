#ifndef _ERRORS_H
#define _ERRORS_H
#include <libraries.h>

/* ----------------------------------------------------------------	*
 *							Error Flags:							*
 * ----------------------------------------------------------------	*/
typedef enum GeneralErrorFlags {
	UNKNOWN_IDENTIFIER					= 0x1,
	EXTRANEOUS_TEXT						= 0x2,
	INVALID_LINE_LENGTH 				= 0x4,
	MEMORY_OVERFLOW						= 0x8
} GeneralErrorFlag;

typedef enum CommaErrorFlags {
	MISSING_COMMA						= 0x1,
	INVALID_COMMA						= 0x2,
	MULTIPLE_CONSECUTIVE_COMMAS			= 0x4
} CommaErrorFlag;

typedef enum LabelErrorFlags {
	INVALID_LABEL_LEN					= 0x1,
	INVALID_LABEL_NAME					= 0x2,
	INVALID_LABEL_SYNTAX				= 0x4,
	EMPTY_LABEL_TAG						= 0x8,
	LABEL_ALREADY_DEFINED				= 0x10,
	MISSING_LABEL_DEFINITION_SUFFIX 	= 0x20
} LabelErrorFlag;

typedef enum InstructionErrorFlags {
	MISSING_OPERANDS					= 0x1,
	TOO_MANY_OPERANDS					= 0x2,
	ILLEGAL_ORIGIN_ADDRESSING_MODE		= 0x4,
	ILLEGAL_DEST_ADDRESSING_MODE		= 0x8,
	ORIGIN_OPERAND_IS_UNDEFINED_LABEL	= 0x10,
	DEST_OPERAND_IS_UNDEFINED_LABEL		= 0x20
} InstructionErrorFlag;

typedef enum DirectiveErrorFlags {
	EMPTY_DIRECTIVE_SENTENCE			= 0x1
} DirectiveErrorFlag;

typedef enum DirectiveDataErrorFlags {
	MISSING_DATA						= 0x1,
	INVALID_DATA_TYPE					= 0x2
} DirectiveDataErrorFlag;

typedef enum DirectiveStrErrorFlags {
	MISSING_STRING						= 0x1,
	MISSING_STRING_TOKEN				= 0x2,
	UNPRINTABLE_STRING_CHARACTER		= 0x4
} DirectiveStrErrorFlag;

typedef enum DirectiveEntryErrorFlags {
	UNDEFINED_LABEL						= 0x1,
	LABEL_ALREADY_DECLARED_EXTERN		= 0x2
} DirectiveEntryErrorFlag;

typedef enum DirectiveExtErrorFlags {
	PREDEFINED_NON_EXTERN_LABEL			= 0x1
} DirectiveExtErrorFlag;

typedef enum AddressModeErrorFlags {
	INVALID_INDEX						= 0x1,
	INVALID_IMMEDIATE_OPERAND			= 0x2
} AddressModeErrorFlag;


#define __ERROR__MISSING_ARGS(EXECUTABLE_NAME) {\
	fprintf(stdout, "Usage: %s <FILE> <FILE> <...>\n"\
			"Aborting...\n", EXECUTABLE_NAME);\
}

#define __ERROR__UNABLE_TO_CREATE_FILE(FILE_NAME) {\
	fprintf(stdout, "Error: unable to open/create file %s\n"\
	"This is an internal program error and is unrelated to the user.\n"\
	"We are truly sorry for the inconvenience.\n", FILE_NAME);\
}

#define __ERROR__FILE_NAME_TOO_LONG(FILE_NAME) {\
	fprintf(stdout, "Error: File \"%s\" exceeded the max length allowed for a file name \n"\
			"(%d)", FILE_NAME, FILENAME_MAX-MAX_FILE_EXTENSION_LEN);\
}

void printGeneralError
(const char *identifier, GeneralErrorFlag flag, uint32_t lineNumber);

void printCommaError(CommaErrorFlag flag, uint32_t lineNumber);

void printLabelError
(const char *labelName, LabelErrorFlag flag, uint32_t lineNumber);

void printInstructionError
(const char *operation, InstructionErrorFlag flag, uint32_t lineNumber);

void printDirectiveError(DirectiveErrorFlag flag, uint32_t lineNumber);

void printDirectiveDataError(DirectiveDataErrorFlag flag, uint32_t lineNumber);

void printDirectiveStringError(DirectiveStrErrorFlag flag, uint32_t lineNumber);

void printDirectiveEntryError
(const char *expr, DirectiveEntryErrorFlag flag, uint32_t lineNumber);

void printDirectiveExternError
(const char *expr, DirectiveExtErrorFlag flag, uint32_t lineNumber);

void printAddressingModeError(AddressModeErrorFlag flag, uint32_t lineNumber);

#endif
