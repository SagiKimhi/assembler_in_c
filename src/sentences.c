#include "addressingModes.h"
#include "errors.h"
#include "libIO.h"
#include "libraries.h"
#include "sizes.h"
#include <sentences.h>

static int isDirectiveToken(const char *token);
static int isInstructionToken(const char *token);
static int validateOperand(char *operand, AddressingMode addressingMode);

SentenceType identifySentenceType(const char *token)
{
	if (!token || !(*token))
		return INVALID_SENTENCE;

	if (isDirectiveToken(token)) {
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
	int operationIndex;
	int addressingMode, i;
	char token[MAX_LINE_LEN+1];
	const char *nextTokenPtr = sentence;

	if (!operation || !sentence || !instructionCounter)
		return 0;

	operationIndex = searchOperation(operation);

	if (operationIndex==FAILURE)
		return 0;

	nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);

	switch (Operations[operationIndex].numOfOperands) {
		case 0:
			if (*token) {
				/* TODO: print error, extraneous text after
				 * operation which requires 0 operands */
				return 0;
			}

			(*instructionCounter) += getOperationMemoryWords(operationIndex);
			return 1;

		case 1:
			if (!(*token)) {
				/* TODO: print error, missing operand */
				return 0;
			}

			if (*token==OPERAND_SEPERATOR) {
				/* TODO: print error, invalid seperator location, 
				 * expected an operand */
				return 0;
			}

			addressingMode = getAddressingMode(token);

			if (!isLegalDestAddressingMode(operationIndex, addressingMode)) {
				/* TODO: print error, invalid operand type. */
				return 0;
			}

			validateOperand(token, addressingMode);
			nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
			(*instructionCounter) += getAdditionalMemoryWords(addressingMode);

			if (*token) {
				/* TODO: print error, extraneous text. */
				return 0;
			}

			return 1;

		case 2:
			if (!(*token)) {
				/* TODO: print error, missing operand */
				return 0;
			}

			for (i=0; i<Operations[operationIndex].numOfOperands; i++) {
				if (*token==OPERAND_SEPERATOR) {
					if (i) {
						/* TODO: print error, multiple consecutive commas */
						return 0;
					}

					/* TODO: print error, invalid seperator location, 
					 * expected an operand */
					return 0;
				}

				addressingMode = getAddressingMode(token);

				if (i) {
					if (!isLegalDestAddressingMode(operationIndex, addressingMode)) {
						/* TODO: print error, invalid operand type. */
						return 0;
					}
				}
				else if (!isLegalOriginAddressingMode(operationIndex, addressingMode)) {
					/* TODO: print error, invalid operand type. */
					return 0;
				}

				validateOperand(token, addressingMode);
				nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
				(*instructionCounter) += getAdditionalMemoryWords(addressingMode);

				if (!i && *token != OPERAND_SEPERATOR) {
					/* TODO: print error, missing seperator. */
					return 0;
				}

				nextTokenPtr += getToken(token, MAX_LINE_LEN+1, nextTokenPtr);
			}

			if (*token == OPERAND_SEPERATOR) {
				/* TODO: print error, multiple consecutive commas */
				return 0;
			}

			return 1;

		default:
			return 0;
	}
}

static int validateOperand(char *operand, AddressingMode addressingMode)
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
static int isDirectiveToken(const char *token)
{
	return (*token == DIRECTIVE_TOKEN_PREFIX);
}

static int isInstructionToken(const char *token)
{
	return (searchOperation(token)!=FAILURE);
}
