#include <firstPass.h>

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
			fprintf(stderr, "Error in line %lu: line too long.\n", lineNumber);
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isLineLabelDefinition(token)) {
			if (isValidLabelDefinition(token, labelName)) {
				/* TODO: invalid label def, print errors */
				fprintf(stderr, "Error in line %lu: invalid label definition '%s'.\n", 
						lineNumber, token);
				validFlag = 0;
			}
			else {
				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));

				if (label!=NULL) {
					/* TODO: Label exists, print error */
				fprintf(stderr, "Error in line %lu: label '%s' already exists.\n", 
						lineNumber, labelName);
					validFlag = 0;
				}
				else
					labelFlag = 1;
			}

			nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
		}

		if (isComment(*token))
			continue;

		if (isComma(*token)) {
			__ERROR__INVALID_COMMA(lineNumber);
			continue;
		}

		switch (identifySentenceType(token)) {
			case INVALID_SENTENCE:
				/* TODO: Print error - unknown identifier. */
				fprintf(stderr, "Error in line %lu: unknown identifier '%s'.\n", 
						lineNumber, token);
				validFlag = 0;
				break;

			case INSTRUCTION_SENTENCE:
				if (!checkInstructionSentence(	token, nextTokenPtr, 
												&instructionCounter, lineNumber))
					validFlag = 0;

				if (labelFlag)
					addTreeNode(symbolTree, labelName, 
								newLabel(instructionCounter, CODE));
				
				break;

			case DIRECTIVE_DATA_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, DATA));

				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_DATA_SENTENCE,
											&dataCounter, lineNumber				))
					validFlag = 0;

				break;

			case DIRECTIVE_STRING_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(dataCounter, STRING));

				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_STRING_SENTENCE,
											&dataCounter, lineNumber				))
					validFlag = 0;

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_ENTRY_SENTENCE,
											&dataCounter, lineNumber				))
					validFlag = 0;
				break;

			case DIRECTIVE_EXTERN_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_ENTRY_SENTENCE,
											&dataCounter, lineNumber				)) {
					/* TODO: print error, invalid extern sentence */
					validFlag = 0;
					break;
				}
				
				getToken(labelName, MAX_LINE_LEN+1, nextTokenPtr);
				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));

				if (label!=NULL && getLabelType(label)!=EXTERN) {
					/* TODO: print error - label already defined in this file */
					fprintf(stderr, "Error in line %lu: label '%s' is already defined "
							"in this file and therefore cannot be defined as extern.\n", 
							lineNumber, token);
					validFlag = 0;
					break;
				}

				addTreeNode(symbolTree, labelName, newLabel(0, EXTERN));
				break;

			case EMPTY_SENTENCE:
				if (labelFlag)
					/* TODO: print warning, empty label definition. */
					fprintf(stderr, "Warning: empty label definition in line %lu for label: '%s'.\n", 
							lineNumber, token);

				break;

			case COMMENT_SENTENCE:
				fprintf(stderr, "Error in line %lu: comments may only start in the "
						"begining of line.\n", lineNumber);
					validFlag = 0;

				break;
		}
	}
	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)-1] == LABEL_DEFINITION_SUFFIX);
}
