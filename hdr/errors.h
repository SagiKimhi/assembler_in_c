#ifndef _ERRORS_H
#define _ERRORS_H
#include <stdio.h>

#ifndef FAILURE
#define FAILURE -1
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif

/* ----------------------------------------------------------------	*
 *							Error Flags:							*
 * ----------------------------------------------------------------	*/
typedef enum GeneralErrorFlags {
	UNKNOWN_KEYWORD = 0x1,
	MEMORY_OVERFLOW = 0x2
} GeneralErrorFlag;

typedef enum CommaErrorFlags {
	MISSING_COMMA = 0x1,
	INVALID_COMMA = 0x2,
	MULTIPLE_CONSECUTIVE_COMMAS = 0x4
} CommaErrorFlag;

typedef enum LabelErrorFlags {
	INVALID_LABEL_LEN = 0x1,
	INVALID_LABEL_NAME = 0x2,
	INVALID_LABEL_SYNTAX = 0x4,
	MISSING_SPACE_AFTER_DEFINITION = 0x8,
	MISSING_LABEL_DEFINITION_SUFFIX = 0x10,
	LABEL_ALREADY_DEFINED = 0x20,
	LABEL_ALREADY_DECLARED_EXTERN = 0x40
} LabelErrorFlag;

typedef enum InstructionErrorFlags {
	MISSING_OPERANDS = 0x1,
	TOO_MANY_OPERANDS = 0x2,
	ILLEGAL_USE_OF_OPERAND = 0x4,
	OPERAND_IS_UNDEFINED_LABEL = 0x8
} InstructionErrorFlag;

typedef enum DirectiveDataErrorFlags {
	MISSING_DATA = 0x1,
	INVALID_DATA_TYPE = 0x2,
	SHORT_INTEGER_OVERFLOW = 0x4
} DirectiveDataErrorFlag;

typedef enum DirectiveStrErrorFlags {
	MISSING_STRING = 0x1,
	STRING_IS_EMPTY = 0x2
} DirectiveStrErrorFlag;

typedef enum DirectiveEntryErrorFlags {
	UNDEFINED_ENTRY_LABEL = 0x1
} DirectiveEntryErrorFlag;

typedef enum DirectiveExtErrorFlags {
	PREDEFINED_EXTERN_LABEL = 0x1
} DirectiveExtErrorFlag;

typedef enum AddressModeErrorFlags {
	INVALID_INDEX = 0x1
} AddressModeErrorFlag;


#define __ERROR__MISSING_ARGS(EXECUTABLE_NAME) {\
	fprintf(stderr, "Usage: %s <FILE> <FILE> <...>\n"\
			"Aborting...\n", EXECUTABLE_NAME);\
}

#define __ERROR__INVALID_FILE_NAME(FILE_NAME) {\
	fprintf(stderr, "Error: file \"%s\" was not found.\n"\
	"Please make sure the correct file path was provided.\n", FILE_NAME);\
}

#define __ERROR__UNABLE_TO_CREATE_FILE {\
	fprintf(stderr, "Error: unable to open/create a file.\n"\
	"This is an internal program error and is unrelated to the user.\n"\
	"We are truly sorry for the inconvenience.\n");\
}

#define __ERROR__FILE_NAME_TOO_LONG(FILE_NAME) {\
	fprintf(stderr, "Error: File \"%s\" exceeded the max length allowed for a file name \n"\
			"(%d)", FILE_NAME, FILENAME_MAX-MAX_FILE_EXTENSION_LEN);\
}

#define __ERROR__INVALID_COMMA(LINE_NUMBER) {\
	fprintf(stderr, "Error: In line number %lu - invalid comma.\n", LINE_NUMBER);\
}
#endif
