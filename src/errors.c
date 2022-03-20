#include "sizes.h"
#include <errors.h>

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
	}
}
