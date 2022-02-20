#include <handleInput.h>

/* getLine: scans a line of input from stream with a maximum length of size and saves it 
 * into buffer, including the newline character. This function also removes trailing
 * whitespaces and does not count trailing whitespaces as part of the line's length.
 * Returns the length of the string saved into buffer upon success.
 * Returns 0 upon failure, or if the newline character was encountered
 * before any non whitespace characters could be saved into buffer.
 * Returns EOF if reached EOF before scanning any non whitespace characters. */
int getLine(char *buffer, int size, FILE *stream)
{
	int i, c;

	/* Skip spaces at the begining of the line */
	if ((c=skipSpaces(stream))==EOF)
		return EOF;

	if (c=='\n')
		return 0;
	
	
	for (i=0, c=fgetc(stream); c!=EOF && c!='\n' && i<(size-1); c=fgetc(stream), i++) {
		buffer[i] = c;

		/* if the last saved character was whitespace, skip all trailing whitespaces.
		 * break if reached either a newline character or EOF while skipping. */
		if (isspace(c) && ((c=skipSpaces(stream))==EOF || c=='\n'))
			break;
	}

	/* If buffer is full and EOF was not reached */
	if (i==(size-1) && c!=EOF) {
		buffer[i]='\0';
		return 0;
	}

	if (c=='\n')
		buffer[i++] = c;

	buffer[i] = '\0';
	return i;
}

/* Scans a string from stream into buffer with a maximum length of size,
 * up until (and excluding) the next whitespace character after the string.
 * Returns the number of characters scanned if successful, 0 upon failure, 
 * or EOF if reached EOF before any characters could be saved into buffer. */
int getWord(char *buffer, size_t size, FILE *stream)
{
	int i, c;

	if (!buffer || !size || !stream)
		return 0;

	/* Skip spaces - including newlines */
	for (c=fgetc(stream); c!=EOF && isspace(c); c=fgetc(stream))
		;
	/* Scan string */
	for (i=0; c!=EOF && !isspace(c) && i<(size-1); c=fgetc(stream))
		buffer[i++] = c;

	if (!i)
		return EOF;
	
	buffer[i] = '\0';
	ungetc(c, stream);
	/* if c is not whitespace then the word is longer than size */
	return (isspace(c)) ? i: 0;
}

/* skipSpaces: skips all trailing spaces in stream up until reaching a
 * newline or a non whitespace character.
 * Returns the latest character encountered, EOF upon failure or if EOF was encountered
 * Non whitespace characters are pushed back into stream with ungetc. */
int skipSpaces(FILE *stream)
{
	int c;

	if (!stream)
		return EOF;

	while (isspace(c=fgetc(stream))) {
		if (c=='\n')
			return c;
	}
	
	if (c == EOF)
		return EOF;

	return ungetc(c, stream);
}
