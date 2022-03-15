#include "addressingModes.h"
#include "binaryTree.h"
#include "labels.h"
#include "libIO.h"
#include "libraries.h"
#include "sentences.h"
#include "sizes.h"
#include <assembler.h>
#include <stdio.h>
#include <stdlib.h>

#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)

static int isLineLabelDefinition(const char *token);
static int startFirstPass(	FILE *inputStream, Tree *symbolTree, 
							uint16_t *instructionCounter, uint16_t *dataCounter);
static int startSecondPass(FILE *inputStream, const char *fileName, Tree *symbolTree, 
							uint16_t *dataCounter, uint16_t *instructionCounter);

int startAssembler(const char *fileName)
{
	/* Variable definitions */
	int validFlag;
	Tree *symbolTree;
	uint16_t instructionCounter, dataCounter;
	FILE *inputStream = openFile(fileName, PREPROCESSED_FILE_EXTENSION, "r");

	if (!inputStream)
		return EXIT_FAILURE;

	/* Variable Instantiations */
	symbolTree = newTree();
	dataCounter = 0;
	instructionCounter = FIRST_MEMORY_ADDRESS;
	validFlag = startFirstPass(inputStream, symbolTree, 
								&instructionCounter, &dataCounter);

	if (validFlag) {
		rewind(inputStream);
		dataCounter += instructionCounter;
		validFlag = startSecondPass(inputStream, fileName, symbolTree, 
									&dataCounter, &instructionCounter);
	}

	fclose(inputStream);
	printTree(stdout, symbolTree, printLabel);
	deleteTree(symbolTree, deleteLabel);
	return (validFlag) ? EXIT_SUCCESS: EXIT_FAILURE;
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
		/* TODO: add error for line being longer than MAX_LINE_LEN */
			fprintf(stderr, "Error in line %lu: line too long.\n", lineNumber);
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isComment(*token))
			continue;

		if (isLineLabelDefinition(token)) {
			if (isValidLabelDefinition(token, labelName)) {
				/* TODO: invalid label def, print errors */
				fprintf(stderr, "Error in line %lu: invalid label definition '%s'.\n", 
						lineNumber, token);
				validFlag = 0;
			}
			else {
				TreeNode *temp = searchTreeNode(symbolTree, labelName);

				if (temp!=NULL) {
					/* TODO: Label exists, print error */
					fprintf(stderr, "Error in line %lu: label '%s' is already defined:\n", 
							lineNumber, labelName);
					printTreeNode(stderr, temp, printLabel);
					validFlag = 0;
				}
				else
					labelFlag = 1;
			}

			nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
		}

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
				if (!checkDirectiveSentence(nextTokenPtr, DIRECTIVE_ENTRY_SENTENCE,
											dataCounter, lineNumber				)) {
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

static int startSecondPass(FILE *inputStream, const char *fileName, Tree *symbolTree, 
							uint16_t *dataCounter, uint16_t *instructionCounter)
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
	dataAddress = (*instructionCounter);
	instructionAddress = FIRST_MEMORY_ADDRESS;
	objectFilePtr = entryFilePtr = externFilePtr = tempDataFilePtr = NULL;

	while (getLine(inputLine, MAX_LINE_LEN+1, inputStream)!=EOF) {
		lineNumber++;
		memoryWordCode = 0;
		memset(operationWords, 0, sizeof(int32_t)*MAX_OPERATION_WORDS);
		memset(additionalWords, 0, sizeof(int32_t)*MAX_ADDITIONAL_WORDS);

		if (isComment(inputLine[0]))
			continue;

		token = strtok(inputLine, OPERAND_SEPERATORS);

		if (isLineLabelDefinition(token))
			token = strtok(NULL, OPERAND_SEPERATORS);

		if (!token)
			continue;

		switch ((sentenceType = identifySentenceType(token))) {
			case INSTRUCTION_SENTENCE:
				if (validFlag && !objectFilePtr) {
					objectFilePtr = openFile(fileName, OBJECT_FILE_EXTENSION, "w");

					if (!objectFilePtr)
						validFlag = 0;

					fprintf(objectFilePtr, "%d %d\n",
							(*instructionCounter)-FIRST_MEMORY_ADDRESS,
							(*dataCounter)-(*instructionCounter));
				}

				operationIndex = searchOperation(token);
				token = strtok(NULL, OPERAND_SEPERATORS);
				operationWords[0] |= ABSOLUTE_CODE | Operations[operationIndex].opCode;

				if (getOperationMemoryWords(operationIndex)>1) {
					operationWords[1] |=	ABSOLUTE_CODE | 
											Operations[operationIndex].functCode;
				}

				isOriginOperand = (Operations[operationIndex].numOfOperands > 1);

				for (i=0; token!=NULL; token=strtok(NULL, OPERAND_SEPERATORS)) {
					addressingMode = getAddressingMode(token);
					operationWords[1] |= (isOriginOperand) ? 
											encodeOriginAddressMode(addressingMode):
											encodeDestAddressMode(addressingMode);

					switch (addressingMode) {
						TreeNode *node;

						case IMMEDIATE:
							scanImmediateExpression(token, &temp);
							additionalWords[i++] |= ABSOLUTE_CODE | temp;
							break;

						case INDEX:
							scanIndexExpression(token, &temp);
							operationWords[1] |= 
								(isOriginOperand) ? 
									encodeOriginRegister(temp): 
									encodeDestRegister(temp);

						case DIRECT:
							node = searchTreeNode(symbolTree, token);
							label = getTreeNodeData(node);

							if (!label) {
								/* TODO: print error, label does not exist. */
								fprintf(stderr, "label %s does not exist\n", token);
								validFlag = 0;
								continue;
							}
							
							switch (getLabelType(label)) {
								case DATA:
								case STRING:
									if (getAddress(label)<(*instructionCounter))
										setLabelAddress(label, getAddress(label)
												+(*instructionCounter));

								case CODE:
									additionalWords[i++] |= RELOCATABLE_CODE | getBaseAddress(label);
									additionalWords[i++] |= RELOCATABLE_CODE | getOffset(label);
									break;

								case EXTERN:
									additionalWords[i++] |= EXTERNAL_CODE | getBaseAddress(label);
									additionalWords[i++] |= EXTERNAL_CODE | getOffset(label);
									break;

								default: break;
							}
							break;

						case REGISTER_DIRECT:
							scanRegister(token, &temp);
							operationWords[1] |= (isOriginOperand) ?
													encodeOriginRegister(temp): 
													encodeDestRegister(temp);
							break;
					}
					isOriginOperand = 0;
				}

				if (validFlag) {
					for (j=0; j<getOperationMemoryWords(operationIndex); j++, instructionAddress++)
						encodeToFile(objectFilePtr, instructionAddress, operationWords[j]);

					for (j=0; j<i; j++, instructionAddress++)
						encodeToFile(objectFilePtr, instructionAddress, additionalWords[j]);
				}

				break;

			case DIRECTIVE_DATA_SENTENCE:
			case DIRECTIVE_STRING_SENTENCE:
				if (validFlag && !tempDataFilePtr) {
					tempDataFilePtr = tmpfile();

					if (!tempDataFilePtr)
						validFlag = 0;
				}


				while ((token=strtok(NULL,OPERAND_SEPERATORS))!=NULL) {
					if (!validFlag)
						continue;

					if (sentenceType==DIRECTIVE_DATA_SENTENCE) {
						sscanf(token, "%hd", &temp);
						memoryWordCode |= ABSOLUTE_CODE | temp;
						encodeToFile(tempDataFilePtr, dataAddress, memoryWordCode);
						dataAddress++;
					}
					else {
						while (*token && !(*token++=='\"' && *token=='\0')) {
							temp = *token;
							memoryWordCode = ABSOLUTE_CODE | temp;
							encodeToFile(tempDataFilePtr, dataAddress, memoryWordCode);
							dataAddress++;
						}

						memoryWordCode = ABSOLUTE_CODE;
						encodeToFile(tempDataFilePtr, dataAddress, memoryWordCode);
					}
				}

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				if ((token=strtok(NULL, OPERAND_SEPERATORS))!=NULL) {
					node = searchTreeNode(symbolTree, token);
					label = getTreeNodeData(node);

					if (!node || !label) {
						/* TODO: print error, label is undefined, 
						 * cannot define an undefined label as entry. */
						validFlag = 0;
					}
				}

				if (!validFlag)
					break;

				if (!entryFilePtr) {
					if (!(entryFilePtr=openFile(fileName, ENTRY_FILE_EXTENSION, "w"))) {
						validFlag = 0;
						break;
					}
				}

				fprintf(entryFilePtr, "%s,%04hu,%04hu\n", getTreeNodeKey(node), 
						getBaseAddress(label), getOffset(label));

				break;

			case DIRECTIVE_EXTERN_SENTENCE:
				if (validFlag && !externFilePtr) {
					externFilePtr = openFile(fileName, EXTERN_FILE_EXTENSION, "w");

					if (!externFilePtr)
						validFlag = 0;
				}

				while ((token=strtok(NULL, OPERAND_SEPERATORS))!=NULL) {
					if (validFlag) {
						TreeNode *node = searchTreeNode(symbolTree, token);
						label = getTreeNodeData(node);

						fprintf(externFilePtr, "%s BASE %04hu\n%s OFFSET %04hu\n", 
								getTreeNodeKey(node), getBaseAddress(label),
								getTreeNodeKey(node), getOffset(label));
					}
				}

				break;

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
	if (!token)
		return 0;

	return (token[strlen(token)-1] == LABEL_DEFINITION_SUFFIX);
}
