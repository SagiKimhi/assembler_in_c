#ifndef _SENTENCES_H
#define _SENTENCES_H
#include <libIO.h>
#include <libraries.h>
#include <operations.h>
#include <labels.h>
#include <errors.h>

#define DIRECTIVE_TOKEN_PREFIX	'.'
#define DATA_SCAN_FORMAT		"%hd"
#define START_DATA_DEFINITION	".data"
#define START_STRING_DEFINITION ".string"
#define START_ENTRY_DEFINITION	".entry"
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

/* identifySentenceType: Identifies and returns the sentence type
 * based on the argument token. This function does not validate
 * the sentence, but solely checks and returns its type based on
 * comparisons made between token and the language's syntax.
 * The token must be the first token in the sentence that is not
 * a label definition.  Upon success, the SentenceType which token 
 * represents is returned. Otherwise, INVALID_SENTENCE is returned. 
 *
 * @param token A pointer to a null terminated character array that
 * contains the sentence's identifier which determines the type of 
 * the sentence by specifying in assembly syntax the action that will
 * be done, i.e. mov/cmp/add/.string/etc. A label definition token does
 * not count as a sentence identifier token!
 * @return Returns the SentenceType of the sentence which is implied by
 * the token argument, if the token is not a predefined assembly keyword/token
 * then INVALID_SENTENCE is returned. */
SentenceType identifySentenceType(const char *token);

/* checkInstructionSentence: Validates an entire instruction sentence and progresses
 * the instruction counter in accordance to the memory words required to represent
 * the operation in machie code as well as in accordance to the addressing modes
 * of each of the operation operands which should reside within the character 
 * pointer sentence. If any error is found, a descriptive message will be
 * printed out sepcifying the line number of the error based on lineNumber
 * and describing the error which occured. Upon error 0 is returned.
 * Otherwise, if the sentence is valid, 1 is returned. 
 *
 * @param operation A pointer to a null terminated character array which
 * holds the token that specifies the operation of the instruction sentence.
 * @param sentence A pointer to a null terminated character array which holds
 * the sentence itself, i.e. the operands and seperators following the operation.
 * @param instructionCounter A pointer to the current instruction counter of the
 * assembler which will be incremented in accordance to the amount of memory words
 * that are required to represent the instruction sentence that's being checked. 
 * @param lineNumber The line number in the source file of the sentence that is
 * currently being checked - this will be used for error printing in case of an error.
 * @return Returns 1 if the sentence is found to be valid, or 0 otherwise. */
int checkInstructionSentence(const char *operation, const char *sentence, 
							uint16_t *IC, uint32_t lineNumber);

/* checkDirectiveSentence: Validates an entire directive sentence and progresses
 * the data counter in accordance to the SentenceType and to the memory words 
 * required to represent the data that is specified in sentence in machie code.
 * If any error is found, a descriptive message will be printed out sepcifying 
 * the line number of the error based on lineNumber and describing the error.
 * Upon error 0 is returned. Otherwise, if the sentence is valid, 1 is returned.
 *
 * @param sentence A pointer to a null terminated character array which holds
 * the sentence itself, i.e. the operands and seperators following the operation.
 * @param sentenceType The sentence type of the sentence that's being checked.
 * This type is typically provided by the identifySentenceType function.
 * @param dataCounter A pointer to the current data counter of the assembler 
 * which will be incremented in accordance to the amount of memory words
 * that are required to represent the data in the sentence that's being checked. 
 * @param lineNumber The line number in the source file of the sentence that is
 * currently being checked - this will be used for error printing.
 * @return Returns 1 if the sentence is found to be valid, or 0 otherwise. */
int checkDirectiveSentence(const char *sentence, SentenceType sentenceType,
							uint16_t *DC, uint32_t lineNumber);

#endif
