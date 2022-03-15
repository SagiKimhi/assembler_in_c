#ifndef _SENTENCES_H
#define _SENTENCES_H
#include <libIO.h>
#include <libraries.h>
#include <operations.h>
#include <labels.h>

#define DIRECTIVE_TOKEN_PREFIX '.'
#define START_DATA_DEFINITION ".data"
#define START_STRING_DEFINITION ".string"
#define START_ENTRY_DEFINITION ".entry"
#define START_EXTERN_DEFINITION ".extern"

typedef enum SentenceTypes {
	INVALID_SENTENCE = FAILURE,
	EMPTY_SENTENCE,
	COMMENT_SENTENCE,
	DIRECTIVE_DATA_SENTENCE,
	DIRECTIVE_STRING_SENTENCE,
	DIRECTIVE_ENTRY_SENTENCE,
	DIRECTIVE_EXTERN_SENTENCE,
	INSTRUCTION_SENTENCE
} SentenceType;

SentenceType identifySentenceType(const char *token);
int checkInstructionSentence(const char *operation, const char *sentence, 
							uint16_t *instructionCounter, uint32_t lineNumber);
int checkDirectiveSentence(const char *sentence, SentenceType type,
							uint16_t *dataCounter, uint32_t lineNumber);

#endif
