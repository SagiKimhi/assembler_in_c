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
	DIRECTIVE_SENTENCE,
	INSTRUCTION_SENTENCE
} SentenceType;

typedef enum DirectiveSentenceTypes {
	NONE_DIRECTIVE_SENTENCE = FAILURE,
	DATA_DIRECTIVE_SENTENCE,
	STRING_DIRECTIVE_SENTENCE,
	ENTRY_DIRECTIVE_SENTENCE,
	EXTERN_DIRECTIVE_SENTENCE
} DirectiveSentenceType;
#endif
