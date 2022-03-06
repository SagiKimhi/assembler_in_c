#include <sentences.h>

SentenceType identifySentenceType(const char *token)
{
	if (!token || !(*token))
		return INVALID_SENTENCE;

	if (isDirectiveToken(token))
		return DIRECTIVE_SENTENCE;

	if (isInstructionToken(token))
		return INSTRUCTION_SENTENCE;

	return INVALID_SENTENCE;
}

DirectiveSentenceType identifyDirectiveSentenceType(const char *token)
{
	if (!token || *token!=DIRECTIVE_TOKEN_PREFIX)
		return NONE_DIRECTIVE_SENTENCE;

	if (!strcmp(START_DATA_DEFINITION))
		return DATA_DIRECTIVE_SENTENCE;

	if (!strcmp(START_STRING_DEFINITION))
		return STRING_DIRECTIVE_SENTENCE;

	if (!strcmp(START_ENTRY_DEFINITION))
		return ENTRY_DIRECTIVE_SENTENCE;

	if (!strcmp(START_EXTERN_DEFINITION))
		return EXTERN_DIRECTIVE_SENTENCE;

	return NONE_DIRECTIVE_SENTENCE;
}

static int isDirectiveToken(const char *token)
{
	return (identifyDirectiveSentenceType(token)!=NONE_DIRECTIVE_SENTENCE);
}

static int isInstructionToken(token)
{
	return (searchOperation(token)!=FAILURE);
}
