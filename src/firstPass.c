#include "labels.h"
#include <firstPass.h>

static int isLineComment(char *buffer);

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

void handleLabelDefinition(const char *label, const char *value, int16_t *instCount, 
		int16_t *dataCount, Tree *symbolTree)
{
	int result;
	SentenceType sentenceType;
	char token[MAX_LINE_LEN+1] = {0};
	const char *nextTokenPtr = value;

	result = isValidLabelDefinition(label);

	if (result) {
		/* TODO: Handle errors. */
		return;
	}

	if (!isspace(*nextTokenPtr)) {
		/* TODO: Error, label definition must end with whitespace */
		return;
	}

	nextTokenPtr += s_getWord(nextTokenPtr, token, MAX_LINE_LEN+1);
	sentenceType = identifySentenceType(token);

	if (sentenceType == INVALID_SENTENCE) {
		/* TODO: Illegal line error. */
		return;
	}

	if (sentenceType == DIRECTIVE_SENTENCE) {
		/* TODO: check directive type, save label,
		 * and increase data counter (optional). */
		DirectiveSentenceType directiveType;

		directiveType = identifyDirectiveSentenceType(token);

		switch (directiveType) {
			case DATA_DIRECTIVE_SENTENCE:
				*dataCount += scanDirectiveData(nextTokenPtr);
				break;

			case STRING_DIRECTIVE_SENTENCE:
				*dataCount += scanDirectiveString(nextTokenPtr);
				break;
			
			case ENTRY_DIRECTIVE_SENTENCE:
				/* A label which appears before an entry directive is meaningless. */
				scanEntry(nextTokenPtr);
				break;

			case EXTERN_DIRECTIVE_SENTENCE:
				/* TODO: Decide which pass handles extern directives 
				 * and add code accordingly. */
				break;

			case NONE_DIRECTIVE_SENTENCE:
				break;
		}
	}

	/* TODO: save label, analyze and validate instruction sentence,
	 * increase instruction counter accordingly. */
	result = checkInstruction(nextTokenPtr, Operations[searchOperation(token)]);

	if (result<=0) {
		/* TODO: Print error, invalid instruction sentence */
	}

	/* TODO: Save label to symbol table */
	*instCount += result;
}

static int isValidData(const char *token)
{
	int data = 0;
	char tempC = 0;
	const char TEST_FORMAT[] = "%hd %c"; /* short int with no trailing nondigit chracters */

	return (sscanf(token, TEST_FORMAT, &data, &tempC)==1);
}
