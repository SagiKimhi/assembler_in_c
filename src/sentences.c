#include "errors.h"
#include "libIO.h"
#include "libraries.h"
#include "sizes.h"
#include <sentences.h>

static int isDirectiveToken(const char *token);
static int isInstructionToken(const char *token);

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

static int isDirectiveToken(const char *token)
{
	return (*token == DIRECTIVE_TOKEN_PREFIX);
}

static int isInstructionToken(const char *token)
{
	return (searchOperation(token)!=FAILURE);
}
