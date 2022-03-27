#ifndef _lib_IO_H
#define _lib_IO_H
#include <libraries.h>
#include <errors.h>

#define COMMENT_PREFIX ';'
#define OPERAND_SEPERATOR ','
#define OPERAND_SEPERATORS ", \t\n"

/* encodeToFile: Encodes param address as an address
 * and decodes the param code to separate 4 bit groups,
 * from most significant the least significant into FILE param stream. */
void encodeToFile(FILE *stream, uint32_t address, int32_t code);

/* openFile: Opens a file from param fileName with file extension 
 * from param fileExtension. On mode from param mode.
 * Will print error in case any encountered.
 * Returns a file descriptor for the certain file. */
FILE *openFile(const char *fileName, const char *fileExtension, const char *mode);

/* deleteFile: Deleteds a file with file name from param fileName
 * and file extension from param fileExtension.
 * Returns 0 if deleted successfully and -1 otherwise. */
int deleteFile(const char *fileName, const char *fileExtension);

/* getToken: Scans a token from the null terminated character array pointer str
 * with a maximum length specified by buffSize and saves it into dest.
 * The scanned token will can only be one of the following:
 * The scanned token will either be a single operand seperator character defined
 * as OPERAND_SEPERATOR in assemblerSyntax.h or a nonwhitespace character array
 * which doesnt contain the OPERAND_SEPERATOR character. 
 * Upon success the function returns the amount of characters skipped in str
 * up until the end of the scanned token, or FAILURE upon failure. */
int getToken(char *dest, size_t buffSize, const char *str);

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
