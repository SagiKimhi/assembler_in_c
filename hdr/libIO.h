#ifndef _lib_IO_H
#define _lib_IO_H
#include <libraries.h>

#define COMMENT_PREFIX ';'
#define OPERAND_SEPERATOR ','
#define OPERAND_SEPERATORS ", \t\n"

void encodeToFile(FILE *stream, uint32_t address, int32_t code);

FILE *openFile(const char *fileName, const char *fileExtension, const char *mode);

int deleteFile(const char *fileName, const char *fileExtension);

int getToken(char *dest, size_t buffSize, const char *str);

/* s_getWord: Scans a single non whitespace word from bufferIn and saves it
 * into bufferOut. Returns the length of the word that was scanned upon success.
 * Returns FAILURE (-1) upon failure, returns 0 if bufferIn's word was longer
 * than the size specified by outSize. */
int s_getWord(const char *bufferIn, char *bufferOut, size_t outSize);

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
