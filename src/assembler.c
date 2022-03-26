#include <assembler.h>

/* ----------------------------------------------------------------	*
 *					Defines, Enums, Consts, etc.					*
 * ----------------------------------------------------------------	*/
#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)
#define NUM_OF_OUTPUT_FILES 3

enum FileIndices {
	OBJECT_FILE,
	ENTRY_FILE,
	EXTERN_FILE
};

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
static int encodeInstruction(const char *fileName, char *sentence, Tree *symbolTree,
								uint16_t *instructionAddress, uint32_t lineNumber);

static int encodeOperand(	const char *fileName, char *token, Tree *symbolTree,
							int operationIndex, int isOriginOperand, 
							int32_t *operationWord, int32_t additionalWords[], 
							uint16_t instructionAddress, int *offset, uint32_t lineNumber);

static int	isLineLabelDefinition(const char *token);

static int	createEntryFile(const char *fileName);

static int	createExternFile(const char *fileName);

static int	createObjectFile(const char *fileName, 
							uint16_t IC, uint16_t DC);

static void closeOutputFiles(void);

static void deleteOutputFiles(const char *fileName);

static void printEntry(TreeNode *node);

static void printExtern(TreeNode *node, uint16_t address);

static void updateSymbolTreeAddresses(TreeNode *symbolTreeRoot, uint16_t IC);

static int	startFirstPass(FILE *inputStream, Tree *symbolTree, 
									uint16_t *IC, uint16_t *DC);

static int	startSecondPass(FILE *inputStream, const char *fileName, 
							Tree *symbolTree, uint16_t IC, uint16_t DC);


/* ----------------------------------------------------------------	*
 *							Static Global Variables					*
 * ----------------------------------------------------------------	*/
static FILE *outputFiles[NUM_OF_OUTPUT_FILES] = {NULL, NULL, NULL};

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
	uint16_t IC, DC;

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
	DC = 0;
	IC = FIRST_MEMORY_ADDRESS;
	
	/* Initiate first pass */
	validFlag = startFirstPass(inputStream, symbolTree, &IC, &DC);

	/* if no errors popped up - rewind file, update counter and 
	 * symbol tree and initiate the second pass. */
	if (validFlag) {
		rewind(inputStream);
		DC += IC;
		updateSymbolTreeAddresses(getRoot(symbolTree), IC);
		validFlag = startSecondPass(inputStream, fileName, symbolTree, IC, DC);
	}

	/* close files and free memory */
	closeOutputFiles();
	fclose(inputStream);
	deleteTree(symbolTree, deleteLabel);

	return (validFlag ? EXIT_SUCCESS: EXIT_FAILURE);
}

static int startFirstPass(FILE *inputStream, Tree *symbolTree, uint16_t *IC, uint16_t *DC)
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
			validFlag = 0;
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isComment(*token))
			continue;

		if (isLineLabelDefinition(token)) {
			if ((result=isValidLabelDefinition(token, labelName))) {
				printLabelError(token, result, lineNumber);
				validFlag = 0;
			}

			else {
				TreeNode *temp = searchTreeNode(symbolTree, labelName);

				if (temp!=NULL) {
					printLabelError(labelName, LABEL_ALREADY_DEFINED, lineNumber);
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
				validFlag = 0;
				break;

			case INSTRUCTION_SENTENCE:
				if (labelFlag)
					addTreeNode(symbolTree, labelName, newLabel(*IC, CODE));
				
				if (!checkInstructionSentence(token, nextTokenPtr, IC, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_DATA_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, newLabel(*DC, DATA));

				if (!checkDirectiveSentence(nextTokenPtr, 
											DIRECTIVE_DATA_SENTENCE, DC, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_STRING_SENTENCE:
				if (labelFlag) 
					addTreeNode(symbolTree, labelName, newLabel(*DC, STRING));

				if (!checkDirectiveSentence(nextTokenPtr, 
											DIRECTIVE_STRING_SENTENCE, DC, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, 
											DIRECTIVE_ENTRY_SENTENCE, DC, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_EXTERN_SENTENCE:
				if (!checkDirectiveSentence(nextTokenPtr, 
											DIRECTIVE_EXTERN_SENTENCE, DC, lineNumber)) {
					validFlag = 0;
					break;
				}
				
				getToken(labelName, MAX_LINE_LEN+1, nextTokenPtr);
				label = getTreeNodeData(searchTreeNode(symbolTree, labelName));

				if (label!=NULL && getLabelType(label)!=EXTERN) {
					printDirectiveExternError(labelName, PREDEFINED_NON_EXTERN_LABEL, lineNumber);
					validFlag = 0;
				}

				else
					addTreeNode(symbolTree, labelName, newLabel(0, EXTERN));

				break;

			case EMPTY_SENTENCE:
				if (labelFlag) {
					printLabelError(labelName, EMPTY_LABEL_TAG, lineNumber);
					validFlag = 0;
				}

				break;

			case COMMENT_SENTENCE:
				printGeneralError(token, UNKNOWN_IDENTIFIER, lineNumber);
				validFlag = 0;
				break;
		}
	}

	if ((*DC + *IC) > MEMSIZE) {
		printGeneralError(NULL, MEMORY_OVERFLOW, lineNumber);
		validFlag = 0;
	}

	return validFlag;
}

static int startSecondPass(FILE *inputStream, const char *fileName, 
							Tree *symbolTree, uint16_t IC, uint16_t DC)
{
	Label *label;
	TreeNode *node;
	SentenceType sentenceType;
	char inputLine[MAX_LINE_LEN+1], token[MAX_LINE_LEN+1];
	char *tokPtr, *ptr;

	int16_t temp;
	uint32_t lineNumber;
	register int32_t memoryWordCode;
	uint16_t instructionAddress, dataAddress;
	int validFlag, temp2, i;
	FILE *tempDataFilePtr;

	/* Variable Instantiations */
	node = NULL;
	label = NULL;
	validFlag = 1;
	lineNumber = temp = 0;
	dataAddress = IC;
	instructionAddress = FIRST_MEMORY_ADDRESS;
	tempDataFilePtr = NULL;

	if ((DC-IC) && !(tempDataFilePtr=tmpfile())) {
		printf("Internal error in second pass: Unable to create data file.\n");
		perror(NULL);
		return 0;
	}

	if (!createObjectFile(fileName, IC, DC))
		return 0;

	while (getLine(inputLine, MAX_LINE_LEN+1, inputStream)!=EOF) {
		lineNumber++;
		memoryWordCode = 0;

		if (isComment(inputLine[0]))
			continue;

		ptr = inputLine;
		temp2 = getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isLineLabelDefinition(token)) {
			getToken(token, MAX_LINE_LEN+1, ptr);
			ptr+=temp2;
		}



		sentenceType=identifySentenceType(token);

		if (sentenceType != INSTRUCTION_SENTENCE)
			tokPtr = strtok(ptr, OPERAND_SEPERATORS);
		
		switch (sentenceType) {
			case INSTRUCTION_SENTENCE:
				/* Encode an instruction sentence: */
				if(!encodeInstruction(fileName, ptr, symbolTree, 
										&instructionAddress, lineNumber))
					validFlag = 0;

				break;
				/*
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
								additionalWords[i++] = ABSOLUTE_CODE | (uint16_t)temp;
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
								printInstructionError(Operations[operationIndex].opName, 
													OPERAND_IS_UNDEFINED_LABEL, lineNumber);
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
										printExtern(externFilePtr, node, instructionAddress+i);
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
					for (j=0; j<getOperationMemoryWords(operationIndex); j++)
						encodeToFile
						(objectFilePtr, instructionAddress++, operationWords[j]);

					for (j=0; j<i; j++)
						encodeToFile
						(objectFilePtr, instructionAddress++, additionalWords[j]);
				}

				break;
				*/

			case DIRECTIVE_DATA_SENTENCE:
				/*encodeDirectiveDataSentence(tempDataFilePtr, ptr, lineNumber);*/
				while ((tokPtr=strtok(NULL,OPERAND_SEPERATORS))!=NULL) {
					if (!validFlag)
						continue;

					sscanf(tokPtr, DATA_SCAN_FORMAT, &temp);
					memoryWordCode |= ABSOLUTE_CODE | (uint16_t)temp;
					encodeToFile(tempDataFilePtr, dataAddress++, memoryWordCode);
				}

				break;

			case DIRECTIVE_STRING_SENTENCE:
				/*encodeDirectiveStringSentence(tempDataFilePtr, ptr, lineNumber);*/
				while ((tokPtr=strtok(NULL,OPERAND_SEPERATORS))!=NULL) {
					for (i=1; tokPtr[i] && tokPtr[i+1]; i++) {
						memoryWordCode = ABSOLUTE_CODE | tokPtr[i];
						encodeToFile(tempDataFilePtr, dataAddress++, memoryWordCode);
					}

					memoryWordCode = ABSOLUTE_CODE;
					encodeToFile(tempDataFilePtr, dataAddress++, memoryWordCode);
				}

				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				/*encodeDirectiveEntrySentence(outputFiles[ENTRY_FILE], ptr, lineNumber);*/
				tokPtr = strtok(NULL, OPERAND_SEPERATORS); /* get entrie's label name */
				node = searchTreeNode(symbolTree, tokPtr);
				label = getTreeNodeData(node);

				if (!node) {
					printDirectiveEntryError(tokPtr, UNDEFINED_LABEL, lineNumber);
					validFlag = 0;
				} 

				else if (getLabelType(label) == EXTERN) {
					printDirectiveEntryError
					(tokPtr, LABEL_ALREADY_DECLARED_EXTERN, lineNumber);
					validFlag = 0;
				}

				else if (!outputFiles[ENTRY_FILE] && !createEntryFile(fileName))
						validFlag = 0;

				if (validFlag)
					printEntry(node);

			default:
				while (strtok(NULL, OPERAND_SEPERATORS)!=NULL)
					;

				break;
		}
	}

	/* If a data file exists copy its contents to the object file */
	if (validFlag && tempDataFilePtr!=NULL) {
		int c;

		rewind(tempDataFilePtr);

		while ((c=fgetc(tempDataFilePtr))!=EOF)
			fputc(c, outputFiles[OBJECT_FILE]);
	}

	if (tempDataFilePtr!=NULL)
		fclose(tempDataFilePtr);

	/* Remove all files if an error was found */
	if (!validFlag)
		deleteOutputFiles(fileName);

	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)-1] == LABEL_DEFINITION_SUFFIX);
}

static int createObjectFile(const char *fileName, uint16_t IC, uint16_t DC)
{
	outputFiles[OBJECT_FILE] = openFile(fileName, OBJECT_FILE_EXTENSION, "w");

	if (!outputFiles[OBJECT_FILE])
		return 0;

	fprintf(outputFiles[OBJECT_FILE], "%d %d\n", IC-FIRST_MEMORY_ADDRESS, DC-IC);
	return 1;
}

static int createEntryFile(const char *fileName)
{
	outputFiles[ENTRY_FILE] = openFile(fileName, ENTRY_FILE_EXTENSION, "w");

	if (!outputFiles[ENTRY_FILE])
		return 0;

	return 1;
}

static int createExternFile(const char *fileName)
{
	outputFiles[EXTERN_FILE] = openFile(fileName, EXTERN_FILE_EXTENSION, "w");

	if (!outputFiles[EXTERN_FILE])
		return 0;

	return 1;
}

static void printEntry(TreeNode *node)
{
	Label *label = getTreeNodeData(node);

	if (!outputFiles[ENTRY_FILE])
		return;

	fprintf(outputFiles[ENTRY_FILE], "%s,%04hu,%04hu\n", 
			getTreeNodeKey(node), getBaseAddress(label), getOffset(label));
}

static void printExtern(TreeNode *node, uint16_t address)
{
	if (!outputFiles[EXTERN_FILE])
		return;

	fprintf(outputFiles[EXTERN_FILE], "%s BASE %04hu\n", getTreeNodeKey(node), address++);
	fprintf(outputFiles[EXTERN_FILE], "%s OFFSET %04hu\n\n", getTreeNodeKey(node), address);
}

static void updateSymbolAddress(Label *label, uint16_t IC)
{
	uint16_t temp;

	switch(getLabelType(label)) {
		case DATA:
		case STRING:
			if ((temp=getAddress(label))<IC)
				setLabelAddress(label, temp+IC);

		default:
			break;
	}
}

static void updateSymbolTreeAddresses(TreeNode *symbolTreeRoot, uint16_t IC)
{
	if (!symbolTreeRoot)
		return;

	updateSymbolAddress(getTreeNodeData(symbolTreeRoot), IC);
	updateSymbolTreeAddresses(getLeftChild(symbolTreeRoot), IC);
	updateSymbolTreeAddresses(getRightChild(symbolTreeRoot), IC);
}

static void closeOutputFiles(void)
{
	int i;

	for (i=0; i<NUM_OF_OUTPUT_FILES; i++) {
		if (outputFiles[i]!=NULL) {
			fclose(outputFiles[i]);
			outputFiles[i] = NULL;
		}
	}
}

static void deleteOutputFiles(const char *fileName)
{
	deleteFile(fileName, OBJECT_FILE_EXTENSION);
	deleteFile(fileName, ENTRY_FILE_EXTENSION);
	deleteFile(fileName, EXTERN_FILE_EXTENSION);
}

static int encodeInstruction(const char *fileName, char *sentence, Tree *symbolTree, 
							uint16_t *instructionAddress, uint32_t lineNumber)
{
	char *token;
	int32_t operationWords[MAX_OPERATION_WORDS];
	int32_t additionalWords[MAX_ADDITIONAL_WORDS];
	int operationIndex, isOriginOperand, validFlag, offset, i;

	if (!(token=strtok(sentence, OPERAND_SEPERATORS)))
		return 0;
	
	/* Variable instantiations */
	offset = i = 0;
	validFlag = 1;
	memset(operationWords,	0, sizeof(int32_t)*MAX_OPERATION_WORDS);
	memset(additionalWords, 0, sizeof(int32_t)*MAX_ADDITIONAL_WORDS);
	
	/* Get the operation based on token */
	if (!isValidOperationIndex(operationIndex=searchOperation(token)))
		return 0;

	/* Encode the opcode of the operation */
	operationWords[0] = ABSOLUTE_CODE | Operations[operationIndex].opCode;
	encodeToFile(outputFiles[OBJECT_FILE], (*instructionAddress)++, operationWords[0]);

	/* return if the operation doesnt require any additional words */
	if (getOperationMemoryWords(operationIndex)==1)
		return 1;

	isOriginOperand		= (Operations[operationIndex].numOfOperands > 1);
	operationWords[1]	= ABSOLUTE_CODE | Operations[operationIndex].functCode;

	while ((token=strtok(NULL, OPERAND_SEPERATORS))!=NULL) {
		if (!encodeOperand(	fileName, token, symbolTree,
							operationIndex, isOriginOperand, 
							&operationWords[1], &additionalWords[offset],
							*instructionAddress, &offset, lineNumber		)) {
			validFlag = 0;
		}

		isOriginOperand = 0;
	}

	if (!validFlag)
		return 0;

	for (i=0; i<getOperationMemoryWords(operationIndex); i++)
		encodeToFile(outputFiles[OBJECT_FILE], (*instructionAddress)++, operationWords[i]);

	for (i=0; i<offset; i++)
		encodeToFile(outputFiles[OBJECT_FILE], (*instructionAddress)++, additionalWords[i]);

	return validFlag;
}

static int encodeOperand(	const char *fileName, char *token, Tree *symbolTree,
					int operationIndex, int isOriginOperand, 
					int32_t *operationWord, int32_t additionalWords[], 
					uint16_t instructionAddress, int *offset, uint32_t lineNumber)
{
	int16_t temp;
	AddressingMode addressingMode;

	addressingMode	= getAddressingMode(token);
	*operationWord |= isOriginOperand ?
						encodeOriginAddressMode(addressingMode):
						encodeDestAddressMode(addressingMode);

	switch (addressingMode) {
		TreeNode *node;
		Label *label;

		case IMMEDIATE:
			scanImmediateExpression(token, &temp);
			additionalWords[0] = ABSOLUTE_CODE | (uint16_t)temp;
			break;

		case INDEX:
			scanIndexExpression(token, &temp);
			*operationWord |= isOriginOperand ? 
								encodeOriginRegister(temp): 
								encodeDestRegister(temp);

		case DIRECT:
			node = searchTreeNode(symbolTree, token);
			label = getTreeNodeData(node);

			if (!label) {
				if (isOriginOperand)
					printInstructionError(Operations[operationIndex].opName, 
											ORIGIN_OPERAND_IS_UNDEFINED_LABEL, lineNumber);
				else
					printInstructionError(Operations[operationIndex].opName,
											DEST_OPERAND_IS_UNDEFINED_LABEL, lineNumber);
				return 0;
			}

			if (getLabelType(label) == EXTERN) {
				if (!outputFiles[EXTERN_FILE] && !createExternFile(fileName))
					return 0;

				additionalWords[0] = EXTERNAL_CODE;
				additionalWords[1] = EXTERNAL_CODE;
				printExtern(node, instructionAddress+(*offset));
			}

			else {
				additionalWords[0] |= RELOCATABLE_CODE | getBaseAddress(label);
				additionalWords[1] |= RELOCATABLE_CODE | getOffset(label);
			}

			break;

		case REGISTER_DIRECT:
			scanRegister(token, &temp);
			*operationWord |= isOriginOperand ?  
								encodeOriginRegister(temp): 
								encodeDestRegister(temp);
			break;
	}

	*offset += getAdditionalMemoryWords(addressingMode);
	return 1;
}
