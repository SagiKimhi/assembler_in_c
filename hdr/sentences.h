#ifndef _SENTENCES_H
#define _SENTENCES_H

#include <libraries.h>
#include <libIO.h>
#include <operations.h>

#define DIRECTIVE_TOKEN_PREFIX '.'
#define START_DATA_DEFINITION ".data"
#define START_STRING_DEFINITION ".string"
#define START_ENTRY_DEFINITION ".entry"
#define START_EXTERN_DEFINITION ".extern"

typedef enum SentenceTypes {
	INVALID_SENTENCE = FAILURE,
	DIRECTIVE_DATA_SENTENCE,
	DIRECTIVE_STRING_SENTENCE,
	DIRECTIVE_ENTRY_SENTENCE,
	DIRECTIVE_EXTERN_SENTENCE,
	INSTRUCTION_SENTENCE
} SentenceType;

SentenceType identifySentenceType(const char *token);

#endif
