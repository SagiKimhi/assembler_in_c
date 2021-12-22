/*  
*	if (feof(file))
*		return 1;
*	scan the line and look for the word syntax macro
*	if (the macro word was found) {
*		if (the following word already exists in our table)
*			ignore everything until "endm" is reached;
*		else {
*			save the following word into a table (see table description later on...);
*			the name of the macro will be the head of a list;
*			while (the next word is not equal to "endm")
*				set the next character combination as the next element in the list;
*		}
*	}
*	else if (the current word exists in our macro table/array) {
*		go to the next element it points to within the table;
*		while (nextElement != NULL)
*			write it into the text file;
*	}
*	write the line into the output file and return to the begining of the funct;
*	
*	For the purpose of the functionality of this source code we will need:
*	1. A dynamic array of (dynamic arrays/pointers to linked lists)
*	2. A string hashtable implementation (maybe stringTable.c/hashTable.c?)
*	3. A counter to keep track of memory usage.
*
*/
#include <macro.h>
#include <linkedList.h>
#include <hashTable.h>


void MacroPreproccessor(FILE *read, FILE *write)
{
	MacroTable macroTable;
	if (!read || !write)
		return;
	initTable(&macroTable, TABLE_SIZE);
	fscanAndExpandMacros(read, write, &macroTable, 0);
	deleteTable(&macroTable);
}

char *fgetWord(char *line, uint8_t len, FILE *fp)
{
	int16_t c;
	uint16_t i, j;
	i=j=0;
	if (!line)
		return NULL;
	while(i<len-1 && (c = fgetc(fp))!=EOF) {
		line[i++] = c;
		if (j && isspace(c))
			break;
		if (!isspace(c))
			j++;
	}
	line[i] = '\0';
	return (!j) ? NULL: line;
}


void fexpandMacro(FILE *rp, FILE *wp, Macro *macro)
{
	int32_t latestReadPos;
	if (!rp || !wp || !macro)
		return;
	latestReadPos = ftell(rp);
	fseek(rp, macro->fStartPos, SEEK_SET);
	while (ftell(rp) < macro->fEndPos)
		fputc(fgetc(rp), wp);
	fseek(rp, latestReadPos, SEEK_SET);
}

uint8_t fscanAndExpandMacros(FILE *rp, FILE *wp, MacroTable *macroTable, uint8_t macroFlag)
{
	char word[MAX_LINE_LEN];
	int16_t i=0;
	Macro *ptr;
	if (!fgetWord(word, MAX_LINE_LEN, rp)) {
		if (feof(rp))
			return EOF;
		if (macroFlag)
			return fscanAndExpandMacros(rp, wp, macroTable, macroFlag);
	}
	while (isspace(word[i]))
			i++;
	if (macroFlag) {
		int32_t pos = ftell(rp);
		if (!word[i])
			exit(EXIT_FAILURE);
		ptr = newMacro(&word[i]);
		ptr->fStartPos = pos;
		while (!feof(rp) && !strstr(word, "endm")) {
			pos = ftell(rp);
			fgetWord(word, MAX_LINE_LEN, rp);
		}
		ptr->fEndPos = pos;
		if (!insert(macroTable, ptr))
			exit(EXIT_FAILURE);
		return fscanAndExpandMacros(rp, wp, macroTable, 0);
	}
	if (strstr(word, "macro")!=NULL)
		return fscanAndExpandMacros(rp, wp, macroTable, 1);
	if ((ptr = search(macroTable, &word[i]))!=NULL)
		fexpandMacro(rp, wp, ptr);
	else 
		fputs(word, wp);
	fprintRestOfLine(rp, wp);
	return fscanAndExpandMacros(rp, wp, macroTable, 0);
}

void fprintRestOfLine(FILE *rp, FILE *wp)
{
	int16_t c;
	while ((c=fgetc(rp))!=EOF && c!='\n')
			fputc(c, wp);
	if (c=='\n')
		fputc(c, wp);
}

Macro *newMacro(char *text)
{
	Macro *ptr = NULL;
	if (!text)
		return NULL;
	if(!(ptr = (Macro *)malloc(sizeof(Macro))))
		return NULL;
	ptr->key = (char *)malloc(sizeof(char)*(strlen(text)+1));
	if (!ptr->key) {
		free(ptr);
		return NULL;
	}
	strcpy(ptr->key, text);
	return ptr;
}

void deleteMacro(Macro *macro)
{
	if (!macro)
		return;
	free(macro->key);
	free(macro);
	macro = NULL;
}