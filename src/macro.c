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


void fexpandMacro(FILE *fp, Macro *macro)
{
	int32_t latestMacroPos, latestFilePos = ftell(fp);
	char tempLine[MAX_LINE_LEN];
	fseek(fp, macro->fStartPos, SEEK_SET);
	while (ftell(fp) < macro->fEndPos)
	{
		if (!fgets(tempLine, MAX_LINE_LEN, fp))
			break;
		latestMacroPos = ftell(fp);
		fseek(fp, latestFilePos, SEEK_SET);
		fprintf(fp, "%s", tempLine);
		latestFilePos = ftell(fp);
		fseek(fp, latestMacroPos, SEEK_SET);
	}
	fseek(fp, latestFilePos, SEEK_SET);
}

uint8_t fscanAndExpandMacros(FILE *fp, MacroTable *macroTable, uint8_t isMacroFlag)
{
	char line[MAX_LINE_LEN], *temp;
	Macro *ptr;
	if (!fscanf(fp, "%s", line)) {
		if (feof(fp))
			return EOF;
		fprintRestOfLine(fp);
		return fscanAndExpandMacros(fp, macroTable, 0);
	}
	if (isMacroFlag) {
		ptr = newMacro(line);
		ptr->fStartPos = ftell(fp);
		while (fgets(line, MAX_LINE_LEN, fp)) {
			if (!(temp=strstr(line, "endm")))
				continue;
			ptr->fEndPos = (ftell(fp)-sizeof("endm"));
			break;
		}
		if (!insert(macroTable, ptr))
			exit(EXIT_FAILURE);
		return fscanAndExpandMacros(fp, macroTable, 0);
	}
	if (strcmp(line, "macro")) {
		if ((ptr = search(macroTable, line)))
			fexpandMacro(fp, ptr);
		else 
			fputs(line, fp);
	}
	fprintRestOfLine(fp);
	return fscanAndExpandMacros(fp, macroTable, 1);
}

void fprintRestOfLine(FILE *fp)
{
	int8_t c;
	while ((c=fgetc(fp))!='\n' && c!=EOF)
			fputc(c, fp);
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