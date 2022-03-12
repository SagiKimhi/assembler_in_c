#include <sentences.h>

static int isInstructionToken(const char *token);
static int validateDirectiveOperand(const char *operand, SentenceType type);
static int validateInstructionOperand(char *operand, AddressingMode addressingMode);

SentenceType identifySentenceType(const char *token)
{
	if (!token)
		return INVALID_SENTENCE;

	if (!(*token))
		return EMPTY_SENTENCE;

	if (*token==COMMENT_PREFIX)
		return COMMENT_SENTENCE;

	if (*token==DIRECTIVE_TOKEN_PREFIX) {
		if (!strcmp(token, START_DATA_DEFINITION))
			return DIRECTIVE_DATA_SENTENCE;

		if (!strcmp(token, START_STRING_DEFINITION))
			return DIRECTIVE_STRING_SENTENCE;

		if (!strcmp(token, START_ENTRY_DEFINITION))
			return DIRECTIVE_ENTRY_SENTENCE;

		if (!strcmp(token, START_EXTERN_DEFINITION))
			return DIRECTIVE_EXTERN_SENTENCE;

		return INVALID_SENTENCE;
	}

	if (isInstructionToken(token))
		return INSTRUCTION_SENTENCE;

	return INVALID_SENTENCE;
}

int checkInstructionSentence(const char *operation, const char *sentence, 
							uint32_t *instructionCounter, uint32_t lineNumber)
{
	char token[MAX_LINE_LEN+1];
	const char *nextTokenPtr = sentence;
	int operationIndex, addressingMode, temp, i;

	if (!operation || !sentence || !instructionCounter)
		return 0;

	operationIndex = searchOperation(operation);

	if (operationIndex==FAILURE)
		return 0;

	nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
	(*instructionCounter) += getOperationMemoryWords(operationIndex);

	for (i=0; i<Operations[operationIndex].numOfOperands; i++) {
		if (!(*token)) {
			/* TODO: print error, missing operands */
			fprintf(stderr, "Error in Line %lu: operation %s requires more operands\n", 
					lineNumber, Operations[operationIndex].opName);
			return 0;
		}

		if (*token==OPERAND_SEPERATOR) {
			/* TODO: print error, invalid comma, expected an operand */;
			fprintf(stderr, "Error in Line %lu: expected an operand but encountered comma\n", 
					lineNumber);
			return 0;
		}

		addressingMode = getAddressingMode(token);
		temp = (!i && Operations[operationIndex].numOfOperands>1) ? 
				isLegalOriginAddressingMode(operationIndex, addressingMode):
				isLegalDestAddressingMode(operationIndex, addressingMode);

		if (!temp) {
			/* TODO: print error, invalid operand. */
			fprintf(stderr, "Error in Line %lu: invalid operand '%s' for operation %s\n", 
					lineNumber, token, Operations[operationIndex].opName);
			return 0;
		}

		validateInstructionOperand(token, addressingMode);
		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
		(*instructionCounter) += getAdditionalMemoryWords(addressingMode);

		if (!i && *token && *token!=OPERAND_SEPERATOR) {
			/* TODO: print error, missing seperator. */
			fprintf(stderr, "Error in Line %lu: Missing comma\n", 
					lineNumber);
			return 0;
		}

		nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
	}

	if (*token) {
		/* TODO: print error, extraneous text. */
		fprintf(stderr, "Error in Line %lu: extraneous text.\n", 
				lineNumber);
		return 0;
	}

	return 1;
}

int checkDirectiveSentence(const char *sentence, SentenceType type,
							uint32_t *dataCounter, uint32_t lineNumber)
{
	char token[MAX_LINE_LEN+1];
	const char *nextTokenPtr = sentence;
	int operandFlag = 1, numOfOperands = 0, temp;

	if (!sentence || !dataCounter)
		return 0;

	nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

	if (!(*token)) {
		/* TODO: print error, empty sentence - missing operands */
			fprintf(stderr, "Error in Line %lu: missing operands in directive sentence\n", 
					lineNumber);
		return 0;
	}

	while (*token) {
		if (type!=DIRECTIVE_DATA_SENTENCE && numOfOperands)
			break;

		if (operandFlag) {
			if (*token==OPERAND_SEPERATOR) {
				/* TODO: print error, invalid seperator, expected operand. */
				fprintf(stderr, "Error in Line %lu: expected an operand but encountered comma\n", 
						lineNumber);
				return 0;
			}

			if (!(temp=validateDirectiveOperand(token, type))) {
				/* TODO: print error, invalid operand. */
				fprintf(stderr, "Error in Line %lu: invalid directive operand '%s'\n", 
						lineNumber, token);
				return 0;
			}
			numOfOperands++;
		}
		else if (*token!=OPERAND_SEPERATOR) {
			/* TODO: print error, expected seperator. */
			fprintf(stderr, "Error in Line %lu: Missing comma\n", 
					lineNumber);
			return 0;
		}

		operandFlag		=	!operandFlag;
		nextTokenPtr	+=	getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
		(*dataCounter)	+=	(type==DIRECTIVE_DATA_SENTENCE ||
							type==DIRECTIVE_STRING_SENTENCE) ? temp: 0;
	}

	if (operandFlag) {
		/* TODO: print error, expected operand. */
		fprintf(stderr, "Error in Line %lu: expected an operand but encountered end of line\n", 
				lineNumber);
		return 0;
	}

	if (*token) {
		/* TODO: print error, extraneous text. */
		fprintf(stderr, "Error in Line %lu: extraneous text.\n", 
				lineNumber);
		return 0;
	}

	return 1;
}

static int validateInstructionOperand(char *operand, AddressingMode addressingMode)
{
	int temp = 0;

	switch (addressingMode) {
		case IMMEDIATE:
			if (scanImmediateExpression(operand, &temp))
				return 1;

			/* TODO: print error, invalid immediate operand */
			return 0;

		case DIRECT:
			if (!isValidLabelTag(operand))
				return 1;

			/* TODO: print error, invalid operand. */
			return 0;

		case INDEX:
			if (scanIndexExpression(operand, &temp)) {
				if (!isValidLabelTag(operand))
					return 1;

				/* TODO: print error, invalid label tag */
				return 0;
			}

			/* TODO: print error, invalid index */
			return 0;

		case REGISTER_DIRECT:
			return 1;

		default:
			return 0;
	}
}

static int validateDirectiveOperand(const char *operand, SentenceType type)
{
	const char DATA_TEST_FORMAT[] = "%hd%c";
	char tempC = 0;
	int16_t tempN = 0;

	switch (type) {
		case DIRECTIVE_DATA_SENTENCE:
			if (sscanf(operand, DATA_TEST_FORMAT, &tempN, &tempC)==1)
				return 1;

			/* TODO: print error, invalid data type,
			 * expected an integer */
			return 0;

		case DIRECTIVE_STRING_SENTENCE:
			tempN = strlen(operand);

			if (*operand=='\"' && tempN>1 && operand[tempN-1]=='\"')
				return (tempN-1);

			/* TODO: print error, missing string delimiters. */
			return 0;

		case DIRECTIVE_ENTRY_SENTENCE:
		case DIRECTIVE_EXTERN_SENTENCE:
			return (!isValidLabelTag(operand)) ? 1: 0;

		default: 
			return 0;
	}
}

static int isInstructionToken(const char *token)
{
	return (searchOperation(token)!=FAILURE);
}
