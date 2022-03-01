#ifndef _HANDLE_INPUT_H
#define _HANDLE_INPUT_H

#include <libraries.h>

/* getLine: scans a line of input from stream with a maximum length of size and saves it 
 * into buffer, including the newline character. This function also removes trailing
 * whitespaces and does not count trailing whitespaces as part of the line's length.
 * Returns the length of the string saved into buffer upon success.
 * Returns 0 upon failure, or if the newline character was encountered
 * before any non whitespace characters could be saved into buffer.
 * Returns EOF if reached EOF before scanning any non whitespace characters. */
int getLine(char *buffer, int size, FILE *stream);

/* Scans a string from stream into buffer with a maximum length of size,
 * up until (and excluding) the next whitespace character after the string.
 * Returns the number of characters scanned if successful, 0 upon failure, 
 * or EOF if reached EOF before any characters could be saved into buffer. */
int getWord(char *buffer, size_t size, FILE *stream);

/* putStreamLine: Prints a line from FILE in at indexIn - out to FILE out
 * at indexOut. Returns 1 if successful, 0 upon failure, or EOF if reached
 * FILE in's End of File. */
int putStreamLine(FILE *in, int32_t indexIn, FILE *out, int32_t indexOut);

/* copyStream: Prints the contents of FILE *in, starting at indexIn up to inLimit,
 * out to FILE *out - at indexOut.
 * Returns 1 if successful, 0 upon failure, or EOF if reached FILE in's End of File. */
int copyStream(FILE *in, int32_t indexIn, int32_t inLimit, FILE *out, int32_t indexOut);

/* skipSpaces: skips all trailing spaces in stream up until reaching a
 * newline or a non whitespace character.
 * Returns the latest character encountered, EOF upon failure or if EOF was encountered
 * Non whitespace characters are pushed back into stream with ungetc. */
int skipSpaces(FILE *stream);
#endif