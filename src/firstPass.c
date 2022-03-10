#include "labels.h"
#include "libIO.h"
#include "sentences.h"
#include "treeNode.h"
#include <firstPass.h>
#include <string.h>

#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)

static int isLineComment(char *buffer);
static int isLineLabelDefinition(const char *token);

int startFirstPass(FILE *inputStream, Tree *symbolTree)
{
	/* Variable Definitions */
	Label *label = NULL;
	char labelName[MAX_LABEL_LEN+1];
	uint16_t instructionCounter, dataCounter;
	int lineNum, labelFlag, validFlag, result;
	char inputLine[MAX_LINE_LEN+1], token[MAX_LINE_LEN+1], *nextTokenPtr;

	/* Variable Instantiations */
	validFlag = 1;
	instructionCounter = 100;
	dataCounter = lineNum = result = 0;

	/* First Pass Implementation */
	while ((result = getLine(inputLine, MAX_LINE_LEN+1, inputStream))!=EOF) {
		lineNum++;

		if (!result) {
		/* TODO: add error for line being longer than MAX_LINE_LEN */
			continue;
		}

		labelFlag = 0;
		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (!(*token) || isComment(*token))
			continue;

		if (isComma(*token)) {
			__ERROR__INVALID_COMMA(lineNum);
			continue;
		}

		if (isLineLabelDefinition(token)) {
			if (isValidLabelDefinition(token)) {
				/* TODO: invalid label def, print errors */
				validFlag = 0;
			}
			else {
				strncpy(labelName, token, strlen(token));
				nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));
				if (label!=NULL) {
					if (getLabelType(label)!=ENTRY)
						/* TODO: Label exists, print error */
						validFlag = 0;
				}
				else if (!(*token)) {
					/* TODO: print warning - empty label definition line */
					continue;
				}
				else
					labelFlag = 1;
			}
		}

		result = identifySentenceType(token);

		switch (result) {
			case INVALID_SENTENCE:
				/* TODO: Print error - unknown command */
				validFlag = 0;
				continue;

			case INSTRUCTION_SENTENCE:
				if (result = isValidOperation(token, nextTokenPtr)) {
					/* TODO: print error according to flags */
					validFlag = 0;
				}

				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(instructionCounter, CODE));

				instructionCounter++;
				continue;

			case DIRECTIVE_DATA_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, DATA));

				if (!checkDataSentence(nextTokenPtr))
					validFlag = 0;

				continue;

			case DIRECTIVE_STRING_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, STRING));

				if (!checkStringSentence(nextTokenPtr))
					validFlag = 0;

				continue;

			case DIRECTIVE_ENTRY_SENTENCE:
		}
	}

	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)] == LABEL_DEFINITION_SUFFIX);
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
			case DIRECTIVE_DATA_SENTENCE:
				*dataCount += scanDirectiveData(nextTokenPtr);
				break;

			case DIRECTIVE_STRING_SENTENCE:
				*dataCount += scanDirectiveString(nextTokenPtr);
				break;
			
			case DIRECTIVE_ENTRY_SENTENCE:
				/* A label which appears before an entry directive is meaningless. */
				scanEntry(nextTokenPtr);
				break;

			case DIRECTIVE_EXTERN_SENTENCE:
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
	const char TEST_FORMAT[] = "%hd%c"; /* short int with no trailing nondigit chracters */

	return (sscanf(token, TEST_FORMAT, &data, &tempC)==1);
}

static int scanDirectiveData(const char *expr)
{
	/* Will match the longest non-empty string of input 
	 * characters not from the set specified in [^...] */
	const char SCAN_OPERAND_FORMAT[] = " %[^,\t\n] ";

	/* Will attempt to scan two commas while ignoring spaces. As long as the 
	 * return value from sscanf is <2 no trailing seperators exist. */
	const char SCAN_SEPERATOR_FORMAT[] = " %[,] %[,]";

	int result, seperatorFlag;
	char token[MAX_LINE_LEN+1];
	const char *nextTokenPtr = expr;

	if (!expr)
		return FAILURE;

	/* TODO: Error: missing actual data */
	if (!(*expr) || (result=s_getWord(expr, token, MAX_LINE_LEN+1))<=0)
		return 0;

	nextTokenPtr += result;

	while (result>0) {
		if (!isValidData(token)) {

		}

		result = s_getWord(nextTokenPtr, token, MAX_LINE_LEN+1);

		if (result!=1 || (*token)!=OPERAND_SEPERATOR)
	}
}
