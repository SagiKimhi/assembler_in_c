#include "addressingModes.h"
#include "sizes.h"
#include <errors.h>

void printGeneralError
(const char *identifier, GeneralErrorFlag flag, uint32_t lineNumber)
{
	switch (flag) {
		case UNKNOWN_IDENTIFIER:
			printf("Error in line %lu: unknown identifier |%s|\n", lineNumber, identifier);
			break;

		case EXTRANEOUS_TEXT:
			printf("Error in line %lu: extraneous text provided.\n", lineNumber);
			break;

		case MEMORY_OVERFLOW:
			printf(	"MEMORY OVERFLOW ERROR: Memory limit exceeded! Maximum memory available"
					"is a total of %d words.\n", MEMSIZE);
	}
}

void printCommaError(CommaErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case MISSING_COMMA:
			printf("missing comma seperator between operands.\n");
			break;

		case INVALID_COMMA:
			printf("invalid comma seperator location.\n");
			break;

		case MULTIPLE_CONSECUTIVE_COMMAS:
			printf(	"multiple consecutive commas encountered, a comma must be "
					"followed by an operand.\n");
			break;

		default: 
			break;
	}
}

void printLabelError
(const char *labelName, LabelErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case INVALID_LABEL_LEN:
			printf(	"label name |%s| surpasses %d characters which is the \n"
					"maximum length allowed for a label name.\n",
					labelName, MAX_LABEL_LEN);
			break;

		case INVALID_LABEL_NAME:
			printf(	"|%s| is a saved keyword and therefore may not be used as a label name.\n",
					labelName);
			break;

		case INVALID_LABEL_SYNTAX:
			printf(	"invalid label name |%s|, a label name must begin with an alphabetic\n"
					"letter and may only consist of alphanumeric characters.\n", labelName);
			break;

		case LABEL_ALREADY_DEFINED:
			printf(	"encountered multiple definitions of label |%s|, "
					"labels must differ in name.\n", labelName);
			break;

		case LABEL_ALREADY_DECLARED_EXTERN:
			printf(	"unable to define label |%s|. This label was already declared extern.\n",
					labelName);
			break;

		case MISSING_LABEL_DEFINITION_SUFFIX:
			printf("label definitions must end with the appropriate suffix ':'\n");
			break;
	}
}

void printInstructionError
(const char *operation, InstructionErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case MISSING_OPERANDS:
			printf("operation |%s| requires more operands.\n", operation);
			break;

		case TOO_MANY_OPERANDS:
			printf("too many operands provided for operation |%s|.\n", operation);
			break;

		case ILLEGAL_ORIGIN_ADDRESSING_MODE:
			printf(	"illegal addressing mode of origin operand for operation "
					"|%s|.\n", operation);
			break;

		case ILLEGAL_DEST_ADDRESSING_MODE:
			printf(	"illegal addressing mode of destination operand for operation "
					"|%s|.\n", operation);
			break;

		case OPERAND_IS_UNDEFINED_LABEL:
			printf("undefined label used as operand for operation |%s|.\n", operation);
			break;
	}
}

void printDirectiveDataError(DirectiveDataErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case MISSING_DATA:
			printf("data directive was given without specifying any data to be saved.\n");
			break;

		case INVALID_DATA_TYPE:
			printf(	"invalid data type for data directive, please make sure you "
					"are only providing short (max 16 bit) integers as data.\n");
			break;
	}
}

void printDirectiveStringError(DirectiveStrErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case MISSING_STRING:
			printf("string directive was given without an actual string to be saved.\n");
			break;

		case MISSING_STRING_TOKEN:
			printf("a string operand must begin and end with the string token \".\n");
			break;

		case UNPRINTABLE_STRING_CHARACTER:
			printf("the specified string operand contains unprintable characters.\n");
			break;
	}
}

void printAddressingModeError(AddressModeErrorFlag flag, uint32_t lineNumber)
{
	printf("Error in line %lu: ", lineNumber);

	switch (flag) {
		case INVALID_INDEX:
			printf(	"the index of an index addressing mode operand must be a register "
					"with a register number of between %d to %d.\n", 
					MIN_INDEX_REGISTER_NUMBER, MAX_REGISTER_NUMBER);
			break;
		
		case INVALID_IMMEDIATE_OPERAND:
			printf("invalid operand for immediate addressing mode.\n"
					"operand must be a short (max 16 bit) integer.\n");
			break;
	}
}
