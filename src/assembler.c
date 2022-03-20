#include "sentences.h"
#include <assembler.h>

/* ----------------------------------------------------------------	*
 *								Defines								*
 * ----------------------------------------------------------------	*/
#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)
#define __WARNING__EMPTY_LABEL_DEF(LABEL, LINE_NUMBER)\
	printf("Warning: meaningless label definition in line number %lu of label |%s| "\
			"due to an empty sentence.\n", LINE_NUMBER, LABEL);

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
static int isLineLabelDefinition(const char *token);

static void printEntry(FILE *stream, TreeNode *node);

static void printExtern(FILE *stream, TreeNode *node, uint16_t address);

static FILE *createEntryFile(const char *fileName);

static FILE *createExternFile(const char *fileName);

static FILE *createObjectFile
(const char *fileName, uint16_t instructionCounter, uint16_t dataCounter);

static void updateSymbolTreeAddresses
(TreeNode *symbolTreeRoot, uint16_t instructionCounter);

static int startFirstPass(	FILE *inputStream, Tree *symbolTree, 
							uint16_t *instructionCounter, uint16_t *dataCounter);

static int startSecondPass(FILE *inputStream, const char *fileName, Tree *symbolTree, 
							uint16_t dataCounter, uint16_t instructionCounter);

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* startAssembler: The main assembler function which initiates the first
 * pass of the assembler as well as the second pass if no errors pop up
 * during the first pass. 
 * The function returns a value of 1 if both passes completed successfully
 * without any errors, otherwise, 0 will be returned.
 * It is worth to mention that warnings do not count as errors and therefore
 * even if a warning was issued, the code will still be assembled and the
 * returned value would still be 1.*/
int startAssembler(const char *fileName)
{
	/* Variable definitions */
	int validFlag;
	Tree *symbolTree;
	FILE *inputStream; 
	uint16_t instructionCounter, dataCounter;

	/* Open the preprocessed source file for reading */
	inputStream = openFile(fileName, PREPROCESSED_FILE_EXTENSION, "r");

	if (!inputStream)
		return EXIT_FAILURE;

	/* Create a new binary tree to store labels */
	if (!(symbolTree = newTree())) {
		fclose(inputStream);
		return EXIT_FAILURE;
	}

	/* Instantiate data and instruction counters */
	dataCounter = 0;
	instructionCounter = FIRST_MEMORY_ADDRESS;
	
	/* Initiate first pass */
	validFlag = startFirstPass
				(inputStream, symbolTree, &instructionCounter, &dataCounter);

	/* if no errors popped up - rewind file, update counter and 
	 * symbol tree and initiate the second pass. */
	if (validFlag) {
		rewind(inputStream);
		dataCounter += instructionCounter;
		updateSymbolTreeAddresses(getRoot(symbolTree), instructionCounter);
		validFlag = startSecondPass(inputStream, fileName, symbolTree, 
									dataCounter, instructionCounter);
	}

	/* close file and free memory */
	fclose(inputStream);
	deleteTree(symbolTree, deleteLabel);

	return ((validFlag) ? EXIT_SUCCESS: EXIT_FAILURE);
}

static int startFirstPass(FILE *inputStream, Tree *symbolTree, 
					uint16_t *instructionCounter, uint16_t *dataCounter)
{
	/* Variable Definitions */
	Label *label = NULL;
	uint32_t lineNumber;
	int labelFlag, validFlag, result;
	char token[MAX_LINE_LEN+1], *nextTokenPtr;
	char inputLine[MAX_LINE_LEN+1], labelName[MAX_LABEL_LEN+1];

	/* Variable Instantiations */
	validFlag = 1;
	lineNumber = result = 0;

	/* First Pass Implementation */
	while ((result = getLine(inputLine, MAX_LINE_LEN+1, inputStream))!=EOF) {
		lineNumber++;
		labelFlag = 0;

		if (!result) {
			printGeneralError(inputLine, INVALID_LINE_LENGTH, lineNumber);
			/* TODO: add error for line being longer than MAX_LINE_LEN 
			fprintf(stderr, "Error in line %lu: line too long.\n", lineNumber);*/
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isComment(*token))
			continue;

		if (isLineLabelDefinition(token)) {
			if ((result=isValidLabelDefinition(token, labelName))) {
				printLabelError(token, result, lineNumber);
				/* TODO: invalid label def, print errors 
				fprintf(stderr, "Error in line %lu: invalid label definition '%s'.\n", 
						lineNumber, token);*/
				validFlag = 0;
			}
			else {
				TreeNode *temp = searchTreeNode(symbolTree, labelName);

				if (temp!=NULL) {
					printLabelError(labelName, LABEL_ALREADY_DEFINED, lineNumber);
					/* TODO: Label exists, print error 
					fprintf(stderr, "Error in line %lu: label '%s' is already defined:\n", 
							lineNumber, labelName);
					printTreeNode(stderr, temp, printLabel); */
					validFlag = 0;
				}
				else
					labelFlag = 1;
			}

			nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
		}

		if (isComma(*token)) {
			printCommaError(INVALID_COMMA, lineNumber);
			continue;
		}

		switch (identifySentenceType(token)) {
			case INVALID_SENTENCE:
				printGeneralError(token, UNKNOWN_IDENTIFIER, lineNumber);
				/* TODO: Print error - unknown identifier. 
				fprintf(stderr, "Error in line %lu: unknown identifier '%s'.\n", 
						lineNumber, token);*/
				validFlag = 0;
				break;

			case INSTRUCTION_SENTENCE:
				if (labelFlag)
					addTreeNode(symbolTree, labelName, 
								newLabel(*instructionCounter, CODE));
				
				if (!checkInstructionSentence(	token, nextTokenPtr, 
												instructionCounter, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_DATA_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(*dataCounter, DATA));

				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_DATA_SENTENCE,
											dataCounter, lineNumber				))
					validFlag = 0;

				break;

			case DIRECTIVE_STRING_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, 
								newLabel(*dataCounter, STRING));

				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_STRING_SENTENCE,
											dataCounter, lineNumber				))
					validFlag = 0;

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_ENTRY_SENTENCE,
											dataCounter, lineNumber				))
					validFlag = 0;
				break;

			case DIRECTIVE_EXTERN_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_EXTERN_SENTENCE,
											dataCounter, lineNumber				)) {
					/* TODO: print error, invalid extern sentence */
					validFlag = 0;
					break;
				}
				
				getToken(labelName, MAX_LINE_LEN+1, nextTokenPtr);
				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));

				if (label!=NULL && getLabelType(label)!=EXTERN) {
					printDirectiveExternError(labelName, PREDEFINED_NON_EXTERN_LABEL, lineNumber);
					/* TODO: print error - label already defined in this file 
					fprintf(stderr, "Error in line %lu: label '%s' is already defined "
							"in this file and therefore cannot be defined as extern.\n", 
							lineNumber, token);*/
					validFlag = 0;
				}
				else
					addTreeNode(symbolTree, labelName, newLabel(0, EXTERN));

				break;

			case EMPTY_SENTENCE:
				if (labelFlag)
					/* TODO: print warning, empty label definition. */
					__WARNING__EMPTY_LABEL_DEF(labelName, lineNumber);

				break;

			case COMMENT_SENTENCE:
				fprintf(stderr, "Error in line %lu: comments may only start in the "
						"begining of line.\n", lineNumber);
					validFlag = 0;

				break;
		}
	}

	if ((*dataCounter + *instructionCounter) > MEMSIZE) {
		/* TODO: print error, max memory exceeded. */
		printGeneralError(NULL, MEMORY_OVERFLOW, lineNumber);
		validFlag = 0;
	
	}

	return validFlag;
}

static int startSecondPass(FILE *inputStream, const char *fileName, Tree *symbolTree, 
							uint16_t dataCounter, uint16_t instructionCounter)
{
	Label *label;
	TreeNode *node;
	SentenceType sentenceType;
	AddressingMode addressingMode;
	char inputLine[MAX_LINE_LEN+1], *token;

	int16_t temp;
	uint32_t lineNumber;
	register int32_t memoryWordCode;
	uint16_t instructionAddress, dataAddress;
	int32_t operationWords[MAX_OPERATION_WORDS];
	int32_t additionalWords[MAX_ADDITIONAL_WORDS];
	int isOriginOperand, operationIndex, validFlag, i, j;
	FILE *objectFilePtr, *entryFilePtr, *externFilePtr, *tempDataFilePtr;

	/* Variable Instantiations */
	node = NULL;
	label = NULL;
	validFlag = 1;
	lineNumber = temp = 0;
	dataAddress = instructionCounter;
	instructionAddress = FIRST_MEMORY_ADDRESS;
	objectFilePtr = entryFilePtr = externFilePtr = tempDataFilePtr = NULL;

	if (dataCounter && !(tempDataFilePtr=tmpfile())) {
		printf("Internal error in second pass: Unable to create data file.\n");
		perror(NULL);
		return 0;
	}

	objectFilePtr = createObjectFile(fileName, instructionCounter, dataCounter);

	while (getLine(inputLine, MAX_LINE_LEN+1, inputStream)!=EOF) {
		lineNumber++;
		memoryWordCode = 0;

		if (isComment(inputLine[0]))
			continue;

		token = strtok(inputLine, OPERAND_SEPERATORS);

		if (isLineLabelDefinition(token))
			token = strtok(NULL, OPERAND_SEPERATORS);

		if (!token)
			continue;

		switch ((sentenceType = identifySentenceType(token))) {
			case INSTRUCTION_SENTENCE:
				memset(operationWords, 0, sizeof(int32_t)*MAX_OPERATION_WORDS);
				memset(additionalWords, 0, sizeof(int32_t)*MAX_ADDITIONAL_WORDS);
				operationIndex = searchOperation(token);
				operationWords[0] =	ABSOLUTE_CODE | 
									Operations[operationIndex].opCode;
				token = strtok(NULL, OPERAND_SEPERATORS);

				if (getOperationMemoryWords(operationIndex)>1) {
					operationWords[1] =	ABSOLUTE_CODE | 
										Operations[operationIndex].functCode;
				}

				isOriginOperand = (Operations[operationIndex].numOfOperands > 1);

				for (i=0; token!=NULL; token=strtok(NULL, OPERAND_SEPERATORS)) {
					addressingMode = getAddressingMode(token);
					operationWords[1] |=	(isOriginOperand) ? 
											encodeOriginAddressMode(addressingMode):
											encodeDestAddressMode(addressingMode);

					switch (addressingMode) {
						TreeNode *node;

						case IMMEDIATE:
							if (validFlag) {
								scanImmediateExpression(token, &temp);
								additionalWords[i++] |= ABSOLUTE_CODE | temp;
							}

							break;

						case INDEX:
							scanIndexExpression(token, &temp);
							operationWords[1] |=	(isOriginOperand) ? 
													encodeOriginRegister(temp): 
													encodeDestRegister(temp);

						case DIRECT:
							node = searchTreeNode(symbolTree, token);
							label = getTreeNodeData(node);

							if (!label) {
								printInstructionError
								(Operations[operationIndex].opName, 
								OPERAND_IS_UNDEFINED_LABEL, lineNumber);
								/* TODO: print error, label does not exist.
								fprintf(stderr, "label %s does not exist\n", token);*/
								validFlag = 0;
							}

							if (!validFlag)
								break;
							
							switch (getLabelType(label)) {
								case DATA:
								case STRING:
								case CODE:
									additionalWords[i++] |= RELOCATABLE_CODE | 
															getBaseAddress(label);
									additionalWords[i++] |= RELOCATABLE_CODE | 
															getOffset(label);
									break;

								case EXTERN:
									if (!externFilePtr	&& 
										!(externFilePtr = createExternFile(fileName)))
											validFlag = 0;

									if (validFlag) {
										additionalWords[i++] = EXTERNAL_CODE;
										additionalWords[i++] = EXTERNAL_CODE;
										printExtern(externFilePtr, node, 
													instructionAddress+i);
									}
									break;

								default:
									break;
							}

							break;

						case REGISTER_DIRECT:
							if (validFlag) {
								scanRegister(token, &temp);
								operationWords[1] |=	(isOriginOperand) ?
														encodeOriginRegister(temp): 
														encodeDestRegister(temp);
							}
							break;
					}
					isOriginOperand = 0;
				}

				if (validFlag) {
					for (j=0; j<getOperationMemoryWords(operationIndex); )
						encodeToFile
						(objectFilePtr, instructionAddress++, operationWords[j++]);

					for (j=0; j<i; )
						encodeToFile
						(objectFilePtr, instructionAddress++, additionalWords[j++]);
				}

				break;

			case DIRECTIVE_DATA_SENTENCE:
			case DIRECTIVE_STRING_SENTENCE:
				while ((token=strtok(NULL,OPERAND_SEPERATORS))!=NULL) {
					if (!validFlag)
						continue;

					if (sentenceType==DIRECTIVE_DATA_SENTENCE) {
						sscanf(token, DATA_SCAN_FORMAT, &temp);
						memoryWordCode |= ABSOLUTE_CODE | temp;
						encodeToFile
						(tempDataFilePtr, dataAddress++, memoryWordCode);
					}
					else {
						for (i=1; token[i] && token[i+1]; i++) {
							memoryWordCode = ABSOLUTE_CODE | token[i];
							encodeToFile
							(tempDataFilePtr, dataAddress++, memoryWordCode);
						}

						memoryWordCode = ABSOLUTE_CODE;
						encodeToFile
						(tempDataFilePtr, dataAddress++, memoryWordCode);
					}
				}

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				token=strtok(NULL, OPERAND_SEPERATORS);
				node = searchTreeNode(symbolTree, token);
				label = getTreeNodeData(node);

				if (!node) {
					printDirectiveEntryError(token, UNDEFINED_LABEL, lineNumber);
					/* TODO: print error, label is undefined, 
					 * cannot define an undefined label as entry. */
					validFlag = 0;
				} 
				else if (getLabelType(label) == EXTERN) {
					printDirectiveEntryError
					(token, LABEL_ALREADY_DECLARED_EXTERN, lineNumber);
					/* TODO: print error, a label may not be 
					 * defined as both extern and entry.*/
					validFlag = 0;
				}
				else if (!entryFilePtr && !(entryFilePtr=createEntryFile(fileName)))
						validFlag = 0;

				if (validFlag)
					printEntry(entryFilePtr, node);

			default:
				while ((token=strtok(NULL, OPERAND_SEPERATORS))!=NULL)
					;

				break;
		}
	}

	if (validFlag && tempDataFilePtr!=NULL) {
		int c;
		rewind(tempDataFilePtr);
		while ((c=fgetc(tempDataFilePtr))!=EOF)
			fputc(c, objectFilePtr);
	}


	if (objectFilePtr!=NULL)
		fclose(objectFilePtr);
	
	if (entryFilePtr!=NULL)
		fclose(entryFilePtr);

	if (externFilePtr!=NULL)
		fclose(externFilePtr);

	if (tempDataFilePtr!=NULL)
		fclose(tempDataFilePtr);

	if (!validFlag) {
		deleteFile(fileName, OBJECT_FILE_EXTENSION);
		deleteFile(fileName, ENTRY_FILE_EXTENSION);
		deleteFile(fileName, EXTERN_FILE_EXTENSION);
	}

	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)-1] == LABEL_DEFINITION_SUFFIX);
}

static FILE *createObjectFile
(const char *fileName, uint16_t instructionCounter, uint16_t dataCounter)
{
	FILE *objectFilePtr = NULL;

		objectFilePtr = openFile(fileName, OBJECT_FILE_EXTENSION, "w");

		if (!objectFilePtr)
			return NULL;

		fprintf(objectFilePtr, "%d %d\n",
				instructionCounter-FIRST_MEMORY_ADDRESS,
				dataCounter-instructionCounter);

		return objectFilePtr;
}

static FILE *createEntryFile(const char *fileName)
{
	FILE *entryFilePtr = openFile(fileName, ENTRY_FILE_EXTENSION, "w");

	return entryFilePtr;
}

static FILE *createExternFile(const char *fileName)
{
	FILE *externFilePtr = openFile(fileName, EXTERN_FILE_EXTENSION, "w");

	return externFilePtr;
}

static void printEntry(FILE *stream, TreeNode *node)
{
	Label *label = getTreeNodeData(node);

	fprintf(stream, "%s,%04hu,%04hu\n", getTreeNodeKey(node),
			getBaseAddress(label), getOffset(label));
}

static void printExtern(FILE *stream, TreeNode *node, uint16_t address)
{
	fprintf(stream, "%s BASE %04hu\n", getTreeNodeKey(node), address++);
	fprintf(stream, "%s OFFSET %04hu\n", getTreeNodeKey(node), address);
}

static void updateSymbolAddress
(Label *label, uint16_t instructionCounter)
{
	uint16_t temp;

	switch(getLabelType(label)) {
		case DATA:
		case STRING:
			if ((temp=getAddress(label))<instructionCounter)
				setLabelAddress(label, temp+instructionCounter);

		default:
			break;
	}
}

static void updateSymbolTreeAddresses
(TreeNode *symbolTreeRoot, uint16_t instructionCounter)
{
	if (!symbolTreeRoot)
		return;

	updateSymbolAddress(getTreeNodeData(symbolTreeRoot), instructionCounter);
	updateSymbolTreeAddresses(getLeftChild(symbolTreeRoot), instructionCounter);
	updateSymbolTreeAddresses(getRightChild(symbolTreeRoot), instructionCounter);
}
