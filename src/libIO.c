#include <libIO.h>

/* s_getWord: Scans a single non whitespace word from bufferIn and saves it
 * into bufferOut. Returns the length of the word that was scanned upon success.
 * Returns FAILURE (-1) upon failure, returns 0 if bufferIn's word was longer
 * than the size specified by outSize. */
int s_getWord(char *bufferIn, char *bufferOut, size_t outSize)
{
	int i, j;

	if (!bufferIn || !bufferOut)
		return FAILURE;

	/* skip all spaces until reaching the first word in bufferOut */
	for (i=0; isspace(bufferIn[i]); i++)
		;

	/* copy all non whitespace characters from bufferIn to bufferOut */
	for (j=0; bufferIn[i] && j<outSize-1 && !isspace(bufferIn[i]); i++, j++)
		bufferOut[j] = bufferIn[i];

	bufferOut[j] = '\0';

	/* if bufferIn[i] is not whitespace then word was longer than outSize */
	if (!isspace(bufferIn[i]) && bufferIn[i]!='\0')
		return 0;

	return j;
}


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
	int inString = 0;

	/* Skip spaces at the begining of the line */
	if ((c=skipSpaces(stream))==EOF)
		return EOF;

	if (c=='\n')
		return 0;
	
	
	for (i=0, c=fgetc(stream); c!=EOF && i<(size-1); i++, c=fgetc(stream)) {
		buffer[i] = c;

		/* TODO: check if escape sequence characters should be supported eg. \" */
		if (c=='\"')
			inString = !inString;

		if (inString)
			continue;

		if (isspace(c))
			if (c=='\n' || skipSpaces(stream)=='\n')
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

	if (!buffer || !size || !stream || feof(stream))
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

	if (isspace(c))
		ungetc(c, stream);

	/* if c is not whitespace or EOF then the word is longer than size */
	return (isspace(c) || c==EOF) ? i: 0;
}

/* skipSpaces: skips all trailing spaces in stream up until reaching a
 * newline or a non whitespace character.
 * Returns the latest character encountered, EOF upon failure or if EOF was encountered
 * Non whitespace characters are pushed back into stream with ungetc. */
int skipSpaces(FILE *stream)
{
	int c;

	if (!stream || feof(stream))
		return EOF;

	while (isspace(c=fgetc(stream))) {
		if (c=='\n')
			return c;
	}
	
	if (c==EOF)
		return EOF;

	return ungetc(c, stream);
}

/* putStreamLine: Prints a line from FILE in at indexIn - out to FILE out at indexOut.
 * Returns 1 if successful, 0 upon failure, or EOF if reached FILE in's End of File. */
int putStreamLine(FILE *in, int32_t indexIn, FILE *out, int32_t indexOut)
{
	int c;

	if (!in || !out)
		return 0;
	
	/* check that indexIn is not at/after EOF */
	if (fseek(in, 0, SEEK_END) || ftell(in)<=indexIn)
		return 0;
	
	/* check that indexOut is not after EOF */
	if (fseek(out, 0, SEEK_END) || ftell(in)<indexOut)
		return 0;

	/* set the appropriate FILE indices */
	if (fseek(in, indexIn, SEEK_SET) || fseek(out, indexOut, SEEK_SET))
		return 0;

	/* Print in to out until reaching a newline or EOF */
	while ((c=getc(in))!=EOF && c!='\n')
		putc(c, out);

	if (c==EOF)
		return EOF;

	/* Print newline and return */
	putc(c, out);
	return 1;
}

/* copyStream: Prints the contents of FILE *in, starting at indexIn up to inLimit,
 * out to FILE *out - at indexOut.
 * Returns 1 if successful, 0 upon failure, or EOF if reached FILE in's End of File. */
int copyStream(FILE *in, int32_t indexIn, int32_t inLimit, FILE *out, int32_t indexOut)
{
	int c;

	if (!in || !out || indexIn>inLimit || indexIn<0)
		return 0;
	
	/* check that inLimit is not after EOF */
	if (fseek(in, 0, SEEK_END) || ftell(in)<=inLimit)
		return 0;
	
	/* check that indexOut is not after EOF */
	if (fseek(out, 0, SEEK_END) || ftell(in)<indexOut)
		return 0;

	/* set the appropriate FILE indices */
	if (fseek(in, indexIn, SEEK_SET) || fseek(out, indexOut, SEEK_SET))
		return 0;

	/* Print in to out until reaching a newline or EOF */
	while ((c=getc(in))!=EOF && ftell(in)<=inLimit)
		putc(c, out);

	return (c==EOF) ? EOF: 1;
}

