#include "assemblerSyntax.h"
#include <assembler.h>

/* ----------------------------------------------------------------	*
 *					Defines, Enums, Consts, etc.					*
 * ----------------------------------------------------------------	*/
#define NUM_OF_OUTPUT_FILES 3
#define isComma(TOKEN)		((TOKEN) == OPERAND_SEPERATOR)
#define isComment(TOKEN)	((TOKEN) == COMMENT_PREFIX)

enum FileIndices {
	OBJECT_FILE,
	ENTRY_FILE,
	EXTERN_FILE
};
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
/* encodeDirectiveDataSentence: Encodes a direct data sentence to the file from param dataFilePtr. 
 * From param sentence get a direct data sentence, with unlimeted amount of data words.
 * This function assumes the data sentence is in the correct format, and will not check for syntax. */
static void encodeDirectiveDataSentence(FILE *dataFilePtr, char *sentence, uint16_t *dataAddress);

/* encodeDirectiveStringSentence: Encodes a direct string sentence to the file from param dataFilePtr.
 * From param string get a string to encode to the file, max size of const MAX_LINE_LEN.
 * Param dataAddress desribes the address of the string sentence in the file.
 * The function assumes the string format is correct and will not check for mistakes. */
static void encodeDirectiveStringSentence(FILE *dataFilePtr, const char *string, uint16_t *dataAddress);

/* encodeEntrySentence: Encodes an entry sentence to the entry file. From param entry
 * checks if the entry is known, and if entry is not external, will throw errors in
 * case of invalid entry. */
static int encodeEntrySentence(char *entry, Tree *symbolTree, uint32_t lineNumber);

/* encodeInstruction: encodes an instruction line to the file from param fileName.
 * From param sentence will ensure a valid operation, will also ensure valid operands.
 * On valid input will encode the operation and operands to the file. 
 * Returns 1 on valid sentence else 0. */
static int encodeInstruction(const char *fileName, char *sentence, Tree *symbolTree,
								uint16_t *instructionAddress, uint32_t lineNumber);

/* encodeOperand: encode an operand to the relevent variable based on addressing mode.
 * Param fileName will be used to create extern file if needed.
 * Returns 0 if label doesn't exist error and 1 otherwise */
static int encodeOperand(	const char *fileName, char *token, Tree *symbolTree,
							int operationIndex, int isOriginOperand, 
							int32_t *operationWord, int32_t additionalWords[], 
							uint16_t instructionAddress, int *offset, uint32_t lineNumber);

/* isLineLabel: Returns whether or not the line defines a label. */
static int	isLineLabelDefinition(const char *token);

/* createEntryFile: Creates an entry file with name from param fileName 
 * return 1 if file created successfully and 0 otherwise */
static int	createEntryFile(const char *fileName);

/* createExternFile: Creates an extern file with name from param fileName 
 * return 1 if file created successfully and 0 otherwise */
static int	createExternFile(const char *fileName);

/* createObjectFile: Creates an object file with name from param fileName 
 * Will write as a header the instruction and data amount, from param IC and DC
 * return 1 if file created successfully and 0 otherwise */
static int	createObjectFile(const char *fileName, 
							uint16_t IC, uint16_t DC);

/* closeOutputFiles: closes all output files safely */
static void closeOutputFiles(void);

/* deleteOutputFiles: Deletes all output files in case there was an error
 * and an output is not required */
static void deleteOutputFiles(const char *fileName);

/* printEntry: writes an entry to the entry file */
static void printEntry(TreeNode *node);

/* printExtern: writes an external label to the extern file
 * based on the extern formatting */
static void printExtern(TreeNode *node, uint16_t address);

/* updateSymbolTreeAddresses: Recursivly add the instruction counter offest 
 * from param IC to all the tree nodes */ 
static void updateSymbolTreeAddresses(TreeNode *symbolTreeRoot, uint16_t IC);

/* startFirstPass: Starts the first pass process,   
 * Will read a post processed file from param inputStream, and checks for a valid input.
 * prints syntax and invalid input errors, if encountered in the file.
 * Will also update the symbol tree from param symbolTree with labels.
 * Returns 1 if process completed successfully and 0 otherwise */  
static int	startFirstPass(FILE *inputStream, Tree *symbolTree, 
									uint16_t *IC, uint16_t *DC);

/* startSecondPass: Starts the second pass process,
 * Will read a post processed file from param inputStream, after it was verified
 * in the first pass. Will encode lines to the necessary files using labels
 * identified in the first pass. Will check for label declarations and will
 * print errors in case issues were encountered.
 * Returns 1 if the process completed successfully and 0 otherwise */
static int	startSecondPass(FILE *inputStream, const char *fileName, 
							Tree *symbolTree, uint16_t IC, uint16_t DC);
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *						Static Global Variables						*
 * ----------------------------------------------------------------	*/
/* outputFiles: represent an array of all the static output files */
static FILE *outputFiles[NUM_OF_OUTPUT_FILES] = {NULL, NULL, NULL};
/* ----------------------------------------------------------------	*/

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
	DC			= 0;
	IC			= FIRST_MEMORY_ADDRESS;
	validFlag	= startFirstPass(inputStream, symbolTree, &IC, &DC);

	/* if no errors popped up - rewind file, update counter and 
	 * symbol tree and initiate the second pass. */
	if (validFlag) {
		DC += IC;
		rewind(inputStream);
		updateSymbolTreeAddresses(getRoot(symbolTree), IC);
		validFlag = startSecondPass(inputStream, fileName, symbolTree, IC, DC);
	}

	/* close files and free memory */
	closeOutputFiles();
	fclose(inputStream);
	deleteTree(symbolTree, deleteLabel);

	return (validFlag ? EXIT_SUCCESS: EXIT_FAILURE);
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
 /* startFirstPass: Starts the first pass process,   
  * Will read a post processed file from param inputStream, and checks for a valid input.
  * prints syntax and invalid input errors, if encountered in the file.
  * Will also update the symbol tree from param symbolTree with labels.
  * Returns 1 if process completed successfully and 0 otherwise */  
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

		if (!result || isComment(inputLine[0]))
			continue;

		if (result>MAX_LINE_LEN) {
			printGeneralError(inputLine, INVALID_LINE_LENGTH, lineNumber);
			validFlag = 0;
			continue;
		}

		nextTokenPtr = inputLine;
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, inputLine);

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
					printLabelError(labelName, EMPTY_LABEL_DEFINITION_SENTENCE, lineNumber);
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

/* startSecondPass: Starts the second pass process,
 * Will read a post processed file from param inputStream, after it was verified
 * in the first pass. Will encode lines to the necessary files using labels
 * identified in the first pass. Will check for label declarations and will
 * print errors in case issues were encountered.
 * Returns 1 if the process completed successfully and 0 otherwise */
static int startSecondPass(FILE *inputStream, const char *fileName, 
							Tree *symbolTree, uint16_t IC, uint16_t DC)
{
	int16_t temp;
	uint32_t lineNumber;
	FILE *tempDataFilePtr;
	int validFlag, result;
	SentenceType sentenceType;
	uint16_t instructionAddress, dataAddress;
	char inputLine[MAX_LINE_LEN+1], token[MAX_LINE_LEN+1], *ptr;


	/* Variable Instantiations */
	validFlag			= 1;
	dataAddress			= IC;
	tempDataFilePtr		= NULL;
	lineNumber = temp	= 0;
	instructionAddress	= FIRST_MEMORY_ADDRESS;

	if ((DC-IC) && !(tempDataFilePtr=tmpfile())) {
		printf("Internal error in second pass: Unable to create temporary data file.\n");
		perror(NULL);
		return 0;
	}

	if ((IC-FIRST_MEMORY_ADDRESS || DC-IC) && !createObjectFile(fileName, IC, DC))
		return 0;

	while ((result=getLine(inputLine, MAX_LINE_LEN+1, inputStream))!=EOF) {
		lineNumber++;

		if (!result || isComment(inputLine[0]) || result>MAX_LINE_LEN)
			continue;

		ptr = inputLine;
		result = getToken(token, MAX_LINE_LEN+1, inputLine);

		if (isLineLabelDefinition(token)) {
			ptr		+= result;
			result	 = getToken(token, MAX_LINE_LEN+1, ptr);
		}

		switch (sentenceType=identifySentenceType(token)) {
			case INSTRUCTION_SENTENCE:
				if(!encodeInstruction(fileName, ptr, symbolTree, 
										&instructionAddress, lineNumber))
					validFlag = 0;

				break;

			case DIRECTIVE_DATA_SENTENCE:
				encodeDirectiveDataSentence(tempDataFilePtr, ptr, &dataAddress);
				break;

			case DIRECTIVE_STRING_SENTENCE:
				encodeDirectiveStringSentence(tempDataFilePtr, ptr+result, &dataAddress);
				break;

			case DIRECTIVE_ENTRY_SENTENCE:
				if (!outputFiles[ENTRY_FILE] && !createEntryFile(fileName)) {
					validFlag = 0;
					break;
				}

				if (!encodeEntrySentence(ptr+result, symbolTree, lineNumber))
					validFlag = 0;

				break;

			default:
				break;
		}
	}

	/* Remove all files if an error was found */
	if (!validFlag) {
		if (tempDataFilePtr!=NULL)
			fclose(tempDataFilePtr);

		deleteOutputFiles(fileName);
		return validFlag;
	}

	/* If a data file exists copy its contents to the object file */
	if (tempDataFilePtr!=NULL) {
		int c;

		rewind(tempDataFilePtr);

		while ((c=fgetc(tempDataFilePtr))!=EOF)
			fputc(c, outputFiles[OBJECT_FILE]);

		fclose(tempDataFilePtr);
	}

	return validFlag;
}

/* isLineLabel: Returns whether or not the line defines a label. */
static int isLineLabelDefinition(const char *token)
{
	return (token[strlen(token)-1] == LABEL_DEFINITION_SUFFIX);
}

/* createObjectFile: Creates an object file with name from param fileName 
 * Will write as a header the instruction and data amount, from param IC and DC
 * return 1 if file created successfully and 0 otherwise */
static int createObjectFile(const char *fileName, uint16_t IC, uint16_t DC)
{
	outputFiles[OBJECT_FILE] = openFile(fileName, OBJECT_FILE_EXTENSION, "w");

	if (!outputFiles[OBJECT_FILE])
		return 0;

	fprintf(outputFiles[OBJECT_FILE], "%d %d\n", IC-FIRST_MEMORY_ADDRESS, DC-IC);
	return 1;
}

/* createEntryFile: Creates an entry file with name from param fileName 
 * return 1 if file created successfully and 0 otherwise */
static int createEntryFile(const char *fileName)
{
	outputFiles[ENTRY_FILE] = openFile(fileName, ENTRY_FILE_EXTENSION, "w");

	if (!outputFiles[ENTRY_FILE])
		return 0;

	return 1;
}

/* createExternFile: Creates an extern file with name from param fileName 
 * return 1 if file created successfully and 0 otherwise */
static int createExternFile(const char *fileName)
{
	outputFiles[EXTERN_FILE] = openFile(fileName, EXTERN_FILE_EXTENSION, "w");

	if (!outputFiles[EXTERN_FILE])
		return 0;

	return 1;
}

/* printEntry: writes an entry to the entry file */
static void printEntry(TreeNode *node)
{
	Label *label = getTreeNodeData(node);

	if (!outputFiles[ENTRY_FILE])
		return;

	fprintf(outputFiles[ENTRY_FILE], "%s,%04hu,%04hu\n", 
			getTreeNodeKey(node), getBaseAddress(label), getOffset(label));
}

/* printExtern: writes an external label to the extern file
 * based on the extern formatting */
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

/* updateSymbolTreeAddresses: Recursivly add the instruction counter offest 
 * from param IC to all the tree nodes */ 
static void updateSymbolTreeAddresses(TreeNode *symbolTreeRoot, uint16_t IC)
{
	if (!symbolTreeRoot)
		return;

	updateSymbolAddress(getTreeNodeData(symbolTreeRoot), IC);
	updateSymbolTreeAddresses(getLeftChild(symbolTreeRoot), IC);
	updateSymbolTreeAddresses(getRightChild(symbolTreeRoot), IC);
}

/* closeOutputFiles: closes all output files safely */
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

/* deleteOutputFiles: Deletes all output files in case there was an error
 * and an output is not required */
static void deleteOutputFiles(const char *fileName)
{
	deleteFile(fileName, OBJECT_FILE_EXTENSION);
	deleteFile(fileName, ENTRY_FILE_EXTENSION);
	deleteFile(fileName, EXTERN_FILE_EXTENSION);
}

/* encodeInstruction: encodes an instruction line to the object file.
 * Param fileName is only used to create extern file in case of need.
 * From param sentence will ensure a valid operation, will also ensure valid operands.
 * On valid input will encode the operation and operands to the file. 
 * Returns 1 on valid sentence else 0. */
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

	encodeToFile(outputFiles[OBJECT_FILE], (*instructionAddress)++, operationWords[1]);

	for (i=0; i<offset; i++)
		encodeToFile(outputFiles[OBJECT_FILE], (*instructionAddress)++, additionalWords[i]);

	return validFlag;
}

/* encodeOperand: encode an operand to the relevent variable based on addressing mode.
 * Param fileName will be used to create extern file if needed.
 * Returns 0 if label doesn't exist error and 1 otherwise */
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
				printExtern(node, instructionAddress+*offset+1);
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
/* encodeDirectiveDataSentence: Encodes a direct data sentence to the file from param dataFilePtr. 
 * From param sentence get a direct data sentence, with unlimeted amount of data words.
 * Param dataAddress describes the address of the data sentence in the file.
 * This function assumes the data sentence is in the correct format, and will not check for syntax. */
static void encodeDirectiveDataSentence(FILE *dataFilePtr, char *sentence, uint16_t *dataAddress)
{
	char *token;
	int16_t temp;
	register int32_t encodedDataWord;
	
	/* Instantiations */
	encodedDataWord = temp = 0;
	token = strtok(sentence, OPERAND_SEPERATORS);

	while ((token=strtok(NULL, OPERAND_SEPERATORS))!=NULL) {
		sscanf(token, DATA_SCAN_FORMAT, &temp);
		encodedDataWord |= ABSOLUTE_CODE | (uint16_t)temp;
		encodeToFile(dataFilePtr, (*dataAddress)++, encodedDataWord);
	}
}
/* encodeDirectiveStringSentence: Encodes a direct string sentence to the file from param dataFilePtr.
 * From param string get a string to encode to the file, max size of const MAX_LINE_LEN.
 * Param dataAddress desribes the address of the string sentence in the file.
 * The function assumes the string format is correct and will not check for mistakes. */
static void encodeDirectiveStringSentence(FILE *dataFilePtr, const char *string, uint16_t *dataAddress)
{
	int i;
	char token[MAX_LINE_LEN+1];
	register int32_t encodedDataWord;


	i = 1;
	encodedDataWord = 0;
	getToken(token, MAX_LINE_LEN+1, string);

	while (token[i+1]) {
		encodedDataWord = ABSOLUTE_CODE | token[i++];
		encodeToFile(dataFilePtr, (*dataAddress)++, encodedDataWord);
	}

	encodedDataWord = ABSOLUTE_CODE;
	encodeToFile(dataFilePtr, (*dataAddress)++, encodedDataWord);
}

/* encodeEntrySentence: Encodes an entry sentence to the entry file. From param entry
 * checks if the entry is known, and if entry is not external, will throw errors in
 * case of invalid entry. Returns 1 on valid entry, and 0 on error. */
static int encodeEntrySentence(char *entry, Tree *symbolTree, uint32_t lineNumber)
{
	Label *label;
	TreeNode *node;
	char token[MAX_LABEL_LEN+1];

	getToken(token, MAX_LABEL_LEN+1, entry);
	node	= searchTreeNode(symbolTree, token);
	label	= getTreeNodeData(node);

	if (!label) {
		printDirectiveEntryError(token, UNDEFINED_LABEL, lineNumber);
		return 0;
	}

	if (getLabelType(label)==EXTERN) {
		printDirectiveEntryError(token, LABEL_ALREADY_DECLARED_EXTERN, lineNumber);
		return 0;
	}

	printEntry(node);
	return 1;
}/* ----------------------------------------------------------------	*/
