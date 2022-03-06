#include "labels.h"
#include <firstPass.h>

static int isLineComment(char *buffer);

typedef LabelType SentenceType;

int startFirstPass(FILE *inputStream, FILE *outputStream, Tree *symbolTree)
{
	/* Variable Definitions */
	char inputLine[MAX_LINE_LEN+1] = {0};
	char firstWord[MAX_LINE_LEN+1] = {0};
	int firstWordSize, result;
	int16_t lineNum, instCount, dataCount;

	/* Variable Instantiations */
	lineNum = 1;
	dataCount = 0;
	instCount = 100;
	firstWordSize = result = 0;

	/* First Pass Implementation */
	while ((result = getLine(inputLine, MAX_LINE_LEN+1, inputStream))!=EOF) {
		if (!result) {
		/* TODO: add error for line being longer than MAX_LINE_LEN */
			return 0;
		}

		lineNum++;
		firstWordSize = s_getWord(inputLine, firstWord, MAX_LINE_LEN+1);

		if (isLineComment(firstWord))
			continue;

		if (isLineLabel(firstWord, firstWordSize)) {
			fprintf(outputStream, "%s", &inputLine[firstWordSize]);
			handleLabelDefinition(firstWord, &inputLine[firstWordSize]);
		}
	}
}

static int isLineComment(char *buffer)
{
	return (buffer[0] == COMMENT_PREFIX ? 1: 0);
}

/* isLineLabel: Returns whether or not the line defines a label. */
int isLineLabel(const char *word, int wordSize)
{
	return word[wordSize] == LABEL_DEFINITION_SUFFIX;
}

void handleLabelDefinition(const char *label, char *value, int16_t *instCount, 
		int16_t *dataCount, Tree *symbolTree)
{
	int result;
	char operation[MAX_LINE_LEN+1] = {0};

	result = isValidLabelDefinition(label);

	if (result) {
		/* TODO: Handle errors. */
		return;
	}

	if (!isspace(*value++)) {
		/* TODO: Error, label definition must end with whitespace */
		return;
	}

	if (*value == DIRECTIVE_PREFIX) {
		/* TODO: Check directive type and define label accordingly */
		return;
	}

	s_getWord(value, operation, MAX_LINE_LEN+1);

	if (searchOperation(operation)==FAILURE) {
		/* TODO: Illegal line, expression is neither a directive or an operation command */
		return;
	}

	/* TODO: save label into symbol table as an instruction. */
	
}

SentenceType identifySentenceType(const char *expr)
{
	
}

