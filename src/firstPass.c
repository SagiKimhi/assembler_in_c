#include "labels.h"
#include "libIO.h"
#include "sentences.h"
#include "treeNode.h"
#include <firstPass.h>
#include <string.h>

#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)

static int isLineLabelDefinition(const char *token);

int startFirstPass(FILE *inputStream, Tree *symbolTree)
{
	/* Variable Definitions */
	Label *label = NULL;
	char labelName[MAX_LABEL_LEN+1];
	int labelFlag, validFlag, result;
	uint32_t instructionCounter, dataCounter, lineNumber;
	char inputLine[MAX_LINE_LEN+1], token[MAX_LINE_LEN+1], *nextTokenPtr;

	/* Variable Instantiations */
	validFlag = 1;
	instructionCounter = 100;
	dataCounter = lineNumber = result = 0;

	/* First Pass Implementation */
	while ((result = getLine(inputLine, MAX_LINE_LEN+1, inputStream))!=EOF) {
		lineNumber++;
		labelFlag = 0;

		if (!result) {
		/* TODO: add error for line being longer than MAX_LINE_LEN */
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (!(*token) || isComment(*token))
			continue;

		if (isComma(*token)) {
			__ERROR__INVALID_COMMA(lineNumber);
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
				if (!checkInstructionSentence(token, nextTokenPtr, 
												&instructionCounter, lineNumber))
					validFlag = 0;

				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(instructionCounter, CODE));

				continue;

			case DIRECTIVE_DATA_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, DATA));

				if (!checkDataSentence(nextTokenPtr, dataCounter))
					validFlag = 0;

				continue;

			case DIRECTIVE_STRING_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, STRING));

				if (!checkStringSentence(nextTokenPtr, dataCounter))
					validFlag = 0;

				continue;

			case DIRECTIVE_ENTRY_SENTENCE:
				nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

				if (!(*token)) {
					/* TODO: print error, empty entry sentence. */
					validFlag = 0;
					continue;
				}

				if (*token == OPERAND_SEPERATOR) {
					/* TODO: print error, invalid comma */
					validFlag = 0;
				}
				else if (isValidLabelTag(token)) {
					/* TODO: print error, invalid label tag */
					validFlag=0;
				}

				nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

				if (*token) {
					/* TODO: print error: too many operands */
					validFlag = 0;
				}

				break;

			case DIRECTIVE_EXTERN_SENTENCE:
				if (checkExternSentence(nextTokenPtr)) {
					/* TODO: print error, invalid extern sentence */
					validFlag = 0;
					continue;
				}
				
				nextTokenPtr += getToken(labelName, MAX_LINE_LEN+1, nextTokenPtr);
				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));

				if (label!=NULL && getLabelType(label)!=EXTERN) {
					/* TODO: print error - label already defined in this file */
					validFlag = 0;
					continue;
				}

				addTreeNode(symbolTree, labelName, newLabel(0, EXTERN));
				break;
		}
	}
	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)] == LABEL_DEFINITION_SUFFIX);
}

static int isValidData(const char *token)
{
	char tempC = 0;
	int16_t data = 0;
	const char TEST_FORMAT[] = "%hd%c"; /* short int with no trailing nondigit chracters */

	return (sscanf(token, TEST_FORMAT, &data, &tempC)==1);
}
