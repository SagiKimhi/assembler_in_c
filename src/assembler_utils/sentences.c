#include <sentences.h>

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
/* isInstructionToken: Checks if the provided token is an existing
 * operation name, returns a none zero value if it is or 0 otherwise. */
static int isInstructionToken(const char *token);

/* validateDirectiveOperand: Validates that the operand specified by the
 * character pointer argument operand is a valid operand based on the provided
 * SentenceType for that operand. 
 * If the operand is found to be valid, then 1 is returned. 
 * Otherwise, A descriptive error message is printed out along with the 
 * line number of the error which is specified by lineNumber, and 0 is returned. */
static int validateDirectiveOperand
(const char *operand, SentenceType type, uint32_t lineNumber);

/* validateInstructionOperand: Validates that the operand specified by the
 * character pointer argument operand is a valid operand based on the provided
 * addressingMode for that operand. If the operand is found to be valid,
 * then 1 is returned. Otherwise, An error is printed out upon error along with 
 * the line number of the error which is specified by lineNumber, 
 * and 0 is returned. */
static int validateInstructionOperand
(char *operand, AddressingMode addressingMode, uint32_t lineNumber);
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* identifySentenceType: Identifies and returns the sentence type
 * based on the argument token. This function does not validate
 * the sentence, but solely checks and returns its type based on
 * comparisons made between token and the language's syntax.
 * The token must be the first token in the sentence that is not
 * a label definition.  Upon success, the SentenceType which token 
 * represents is returned. Otherwise, INVALID_SENTENCE is returned. */
SentenceType identifySentenceType(const char *token)
{
	if (!token)
		return INVALID_SENTENCE;

	if (!(*token))
		return EMPTY_SENTENCE;

	if (*token==COMMENT_PREFIX)
		return COMMENT_SENTENCE;

	if (*token==DIRECTIVE_TOKEN_PREFIX) {
		if (!strcmp(token+1, START_DATA_DEFINITION))
			return DIRECTIVE_DATA_SENTENCE;

		if (!strcmp(token+1, START_STRING_DEFINITION))
			return DIRECTIVE_STRING_SENTENCE;

		if (!strcmp(token+1, START_ENTRY_DEFINITION))
			return DIRECTIVE_ENTRY_SENTENCE;

		if (!strcmp(token+1, START_EXTERN_DEFINITION))
			return DIRECTIVE_EXTERN_SENTENCE;

		return INVALID_SENTENCE;
	}

	if (isInstructionToken(token))
		return INSTRUCTION_SENTENCE;

	return INVALID_SENTENCE;
}

/* checkInstructionSentence: Validates an entire instruction sentence and progresses
 * the instruction counter in accordance to the memory words required to represent
 * the operation in machie code as well as in accordance to the addressing modes
 * of each of the operation operands which should reside within the character 
 * pointer sentence. If any error is found, a descriptive message will be
 * printed out sepcifying the line number of the error based on lineNumber
 * and describing the error which occured. Upon error 0 is returned.
 * Otherwise, if the sentence is valid, 1 is returned. */
int checkInstructionSentence(const char *operation, const char *sentence, 
							uint16_t *IC, uint32_t lineNumber)
{
	const char *nextTokenPtr;
	char token[MAX_LINE_LEN+1];
	int isOriginOperand, isLegalOperand, isOperand;
	int operationIndex, addressingMode, operands;

	/* Error checking */
	if (!operation || !sentence || !IC)
		return 0;

	operationIndex	= searchOperation(operation);

	if (!isValidOperationIndex(operationIndex)) {
		return 0;
	}
	
	/* variable instantiations */
	isOperand		= 1;
	nextTokenPtr	= sentence;
	isOriginOperand	= (Operations[operationIndex].numOfOperands > 1);
	(*IC)			+= getOperationMemoryWords(operationIndex);
	nextTokenPtr	+= getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

	/* Begin scanning the operands and seperators */
	for (operands=0; operands<Operations[operationIndex].numOfOperands; isOperand=!isOperand) {

		if (isOperand) {
			if (!(*token)) {
				printInstructionError(operation, MISSING_OPERANDS, lineNumber);
				return 0;
			}

			if (*token==OPERAND_SEPERATOR) {
				printCommaError(INVALID_COMMA, lineNumber);
				return 0;
			}

			addressingMode	=	getAddressingMode(token);
			isLegalOperand	=	(isOriginOperand) ? 
				isLegalOriginAddressingMode(operationIndex, addressingMode):
				isLegalDestAddressingMode(operationIndex, addressingMode);

			if (!isLegalOperand) {
				if (isOriginOperand)
					printInstructionError(operation, ILLEGAL_ORIGIN_ADDRESSING_MODE, lineNumber);
				else
					printInstructionError(operation, ILLEGAL_DEST_ADDRESSING_MODE, lineNumber);

				return 0;
			}

			if (!validateInstructionOperand(token, addressingMode, lineNumber))
				return 0;

			(*IC)			+= getAdditionalMemoryWords(addressingMode);
			isOriginOperand  = 0;
			operands++;
		}

		else if (*token!=OPERAND_SEPERATOR) {
			printCommaError(MISSING_COMMA, lineNumber);
			return 0;
		}

		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
	}

	if (*token) {
		printGeneralError(token, EXTRANEOUS_TEXT, lineNumber);
		return 0;
	}

	return 1;
}

/* checkDirectiveSentence: Validates an entire directive sentence and progresses
 * the data counter in accordance to the SentenceType and to the memory words 
 * required to represent the data that is specified in sentence in machie code.
 * If any error is found, a descriptive message will be printed out sepcifying 
 * the line number of the error based on lineNumber and describing the error.
 * Upon error 0 is returned. Otherwise, if the sentence is valid, 1 is returned. */
int checkDirectiveSentence(const char *sentence, SentenceType type,
							uint16_t *DC, uint32_t lineNumber)
{
	const char *nextTokenPtr;
	char token[MAX_LINE_LEN+1];
	int isOperand, operandCount, tempResult;

	if (!sentence || !DC)
		return 0;

	nextTokenPtr	 = sentence;
	nextTokenPtr	+= getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
	isOperand		 = 1;
	operandCount	 = 0;

	if (!(*token)) {
		printDirectiveError(EMPTY_DIRECTIVE_SENTENCE, lineNumber);
		return 0;
	}

	while (*token) {
		if (type!=DIRECTIVE_DATA_SENTENCE && operandCount)
			break;

		if (isOperand) {
			if (*token==OPERAND_SEPERATOR) {
				printCommaError(INVALID_COMMA, lineNumber);
				return 0;
			}

			if (!(tempResult=validateDirectiveOperand(token, type, lineNumber)))
				return 0;

			operandCount++;
			(*DC)	+= (type==DIRECTIVE_DATA_SENTENCE || 
						type==DIRECTIVE_STRING_SENTENCE) ? tempResult: 0;
		}

		else if (*token!=OPERAND_SEPERATOR) {
			printCommaError(MISSING_COMMA, lineNumber);
			return 0;
		}

		isOperand		=	!isOperand;
		nextTokenPtr	+=	getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
	}

	if (*token) {
		printGeneralError(token, EXTRANEOUS_TEXT, lineNumber);
		return 0;
	}

	return 1;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
/* validateInstructionOperand: Validates that the operand specified by the
 * character pointer argument operand is a valid operand based on the provided
 * addressingMode for that operand. If the operand is found to be valid,
 * then 1 is returned. Otherwise, An error is printed out upon error along with 
 * the line number of the error which is specified by lineNumber, 
 * and 0 is returned. */
static int validateInstructionOperand
(char *operand, AddressingMode addressingMode, uint32_t lineNumber)
{
	int16_t temp = 0;
	int flags = 0;

	switch (addressingMode) {
		case IMMEDIATE:
			if (scanImmediateExpression(operand, &temp))
				return 1;

			printAddressingModeError(INVALID_IMMEDIATE_OPERAND, lineNumber);
			return 0;

		/* An index addressing mode is a combination of direct and
		 * register direct, therefore there is no break after the condition
		 * and we move on to checking operand as direct as well. */
		case INDEX:
			if (!scanIndexExpression(operand, &temp)) {
				printAddressingModeError(INVALID_INDEX, lineNumber);
				return 0;
			}

		case DIRECT:
			if (!(flags=isValidLabelTag(operand)))
				return 1;

			printLabelError(operand, flags, lineNumber);
			return 0;

		case REGISTER_DIRECT:
			return 1;

		default:
			return 0;
	}
}

/* validateDirectiveOperand: Validates that the operand specified by the
 * character pointer argument operand is a valid operand based on the provided
 * SentenceType for that operand. 
 * If the operand is found to be valid, then 1 is returned. 
 * Otherwise, A descriptive error message is printed out along with the 
 * line number of the error which is specified by lineNumber, and 0 is returned. */
static int validateDirectiveOperand
(const char *operand, SentenceType type, uint32_t lineNumber)
{
	const char DATA_TEST_FORMAT[] = DATA_SCAN_FORMAT"%c";
	int flags = 0;
	char tempC = 0;
	int16_t tempN = 0;

	switch (type) {
		case DIRECTIVE_DATA_SENTENCE:
			if (sscanf(operand, DATA_TEST_FORMAT, &tempN, &tempC)==1)
				return 1;

			printDirectiveDataError(INVALID_DATA_TYPE, lineNumber);
			return 0;

		case DIRECTIVE_STRING_SENTENCE:
			if (*operand=='\"') {
				for (; operand[tempN]; tempN++) {
					if (!isprint(operand[tempN])) {
						printDirectiveStringError(UNPRINTABLE_STRING_CHARACTER, lineNumber);
						return 0;
					}
				}

				if (tempN && !operand[tempN] && operand[tempN-1]=='\"')
					/* Length of the initial string minus the string delimiters */
					return tempN-1;
			}

			printDirectiveStringError(MISSING_STRING_TOKEN, lineNumber);
			return 0;

		case DIRECTIVE_ENTRY_SENTENCE:
		case DIRECTIVE_EXTERN_SENTENCE:
			if (!(flags=isValidLabelTag(operand)))
				return 1;

			printLabelError(operand, flags, lineNumber);

		default: 
			return 0;
	}
}

/* isInstructionToken: Checks if the provided token is an existing
 * operation name, returns a none zero value if it is or 0 otherwise. */
static int isInstructionToken(const char *token)
{
	return (searchOperation(token)!=FAILURE);
}
/* ----------------------------------------------------------------	*/
