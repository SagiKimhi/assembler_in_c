#include <libIO.h>

/* encodeToFile: Encodes param address as an address
 * and decode the param code to separate 4 bit groups,
 * from most significant the least significant. */
void encodeToFile(FILE *stream, uint32_t address, int32_t code)
{
	const char		FIRST_HEX_GROUP		= 'A';
	const char		HEX_GROUP_SEPERATOR	= '-';
	const int32_t	FIRST_FOUR_BITS		= 0xf;
	const int		BITS_IN_HALF_BYTE	= 4;

	char	hexGroup		= FIRST_HEX_GROUP;
	int		shiftDistance	= MEM_CELL_SIZE;

	if (!stream)
		return;

	if (address>=FIRST_MEMORY_ADDRESS)
		fprintf(stream, "%04lu ", address);

	while (shiftDistance) {
		shiftDistance -= BITS_IN_HALF_BYTE;

		fprintf(stream, "%c%lx", hexGroup++, code >> shiftDistance & FIRST_FOUR_BITS);

		if (shiftDistance)
			fputc(HEX_GROUP_SEPERATOR, stream);
		else
			fputc('\n', stream);
	}
}

/* openFile: Opens a file from param fileName with file extension 
 * from param fileExtension. On mode from param mode.
 * Will print error in case any encountered.
 * Returns a file descriptor for the certain file. */
FILE *openFile(const char *fileName, const char *fileExtension, const char *mode)
{
	FILE *fp;
	char *newFileName;
	int fileNameLen, extensionLen;

	if (!fileName || !fileExtension || !mode)
		return NULL;

	fileNameLen = strlen(fileName);
	extensionLen = strlen(fileExtension);

	if ((fileNameLen+extensionLen)>FILENAME_MAX) {
		__ERROR__FILE_NAME_TOO_LONG(fileName);
		return NULL;
	}

	newFileName = (char *) malloc(fileNameLen+extensionLen+1);

	if (!newFileName)
		return NULL;

	sprintf(newFileName, "%s%s", fileName, fileExtension);

	if (!(fp=fopen(newFileName, mode))) {
		printf("Error: unable to open/create file '%s'.\n", newFileName);
		perror(NULL);
	}

	free(newFileName);
	return fp;
}

/* deleteFile: Deleteds a file with file name from param fileName
 * and file extension from param fileExtension.
 * Returns 0 if deleted successfully and -1 otherwise. */
int deleteFile(const char *fileName, const char *fileExtension)
{
	char *newFileName;
	int fileNameLen, extensionLen;

	if (!fileName || !fileExtension)
		return -1;

	fileNameLen = strlen(fileName);
	extensionLen = strlen(fileExtension);

	if ((fileNameLen+extensionLen)>FILENAME_MAX) {
		__ERROR__FILE_NAME_TOO_LONG(fileName);
		return -1;
	}

	newFileName = (char *) malloc(fileNameLen+extensionLen+1);

	if (!newFileName)
		return -1;

	sprintf(newFileName, "%s%s", fileName, fileExtension);

	if (remove(newFileName)) {
		free(newFileName);
		return -1;
	}

	free(newFileName);
	return 0;
}

int getToken(char *dest, size_t buffSize, const char *str)
{
	int inString = 0, i = 0, j = 0;

	if (!str || !dest || !buffSize)
		return FAILURE;

	while (isspace(str[i])) 
		i++;

	while (j<(buffSize-1) && str[i] && str[i]!='\n') {
		if (str[i] == '\"' && !inString)
			inString = !inString;

		if (!inString && (isspace(str[i]) || str[i]==OPERAND_SEPERATOR))
			break;

		dest[j++] = str[i++];
	}

	if (str[i]==OPERAND_SEPERATOR && !j)
		dest[j++] = str[i++];

	dest[j] = '\0';
	return i;
}

/* getLine: scans a line of input from stream with a maximum length of size and saves it 
 * into buffer, including the newline character. This function also removes trailing
 * whitespaces and does not count trailing whitespaces as part of the line's length.
 * Returns the length of the line excluding trailing whitespaces.
 * Returns 0 if the newline character was encountered before any non whitespace 
 * characters could be saved into buffer.
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
	
	
	for (i=0, c=fgetc(stream); c!=EOF && c!='\n' && i<(size-1); i++, c=fgetc(stream)) {
		/* This condition allows support of escape sequence characters by 
		 * not changing inString state when reside inside a string definition */
		if (c=='\"' && !inString)
			inString = !inString;

		buffer[i] = c;

		if (inString)
			continue;

		if (isspace(c) && skipSpaces(stream)=='\n')
				break;
	}

	/* If buffer is full and EOF was not reached */
	if (i==(size-1) && c!=EOF && c!='\n') {
		buffer[i]='\0';

		while ((c=fgetc(stream))!=EOF && c!='\n')
			i++;

		return i;
	}

	buffer[i] = '\0';
	return i;
}

/* Scans a string from stream into buffer with a maximum length of size,
 * up until (and excluding) the next whitespace character after the string.
 * Returns the number of characters scanned if successful, 0 upon failure, 
 * or EOF if reached EOF before any characters could be saved into buffer. */
int getWord(char *buffer, size_t size, FILE *stream)
{
	int inString, i, c;

	if (!buffer || !size || !stream || feof(stream))
		return EOF;

	/* Skip spaces - including newlines */
	for (c=fgetc(stream); c!=EOF && isspace(c); c=fgetc(stream))
		;
	/* Scan string */
	for (i=inString=0; c!=EOF && i<(size-1); c=fgetc(stream), i++) {
		buffer[i] = c;

		if (!inString && c=='\"') {
			inString = 1;
		}
		else if (inString) {
			if (c=='\n')
				break;
		}
		else if (isspace(c))
			break;
	}

	buffer[i] = '\0';

	if (!i)
		return 0;
	
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

