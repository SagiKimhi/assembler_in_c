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

void MacroPreproccessor(FILE *read, FILE *write)
{
	HashTable *hashTable;
	if (!read || !write)
		return;
	initTable(hashTable, TABLE_SIZE);
	fscanAndExpandMacros(read, write, hashTable, 0);
	deleteTable(hashTable);
}

int fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, HashTable *hashTable, int macroFlag)
{
	int i=0;
	Macro *macro;
	char word[MAX_LINE_LEN];

	if (fscanf(readPtr, " %81s ", word) != 1) 
		return -1;

	if (macroFlag) {
		int tempPos;
		char tempWord[MAX_LINE_LEN];

		macro = newMacro();
		macro->startPos = ftell(readPtr);

		while (fscanf(readPtr, " %81s ", tempWord)==1 && strcmp(tempWord, "endm"))
			tempPos = ftell(readPtr);

		macro->endPos = tempPos;
		if (!insert(hashTable, macro, word))
			exit(EXIT_FAILURE);

		return fscanAndExpandMacros(readPtr, writePtr, hashTable, 0);
	}

	if (!strcmp(word, "macro"))
		return fscanAndExpandMacros(readPtr, writePtr, hashTable, 1);

	if ((macro = (Macro *) search(hashTable, word))!=NULL)
		fexpandMacro(readPtr, writePtr, macro);
	else
		fputs(word, writePtr);

	fprintRestOfLine(readPtr, writePtr);
	return fscanAndExpandMacros(readPtr, writePtr, hashTable, 0);
}


void fexpandMacro(FILE *readPtr, FILE *writePtr, Macro *macro)
{
	int32_t latestReadPos;

	if (!readPtr || !writePtr || !macro)
		return;

	latestReadPos = ftell(readPtr);
	fseek(readPtr, macro->startPos, SEEK_SET);

	while (ftell(readPtr) < macro->endPos)
		fputc(fgetc(readPtr), writePtr);

	fseek(readPtr, latestReadPos, SEEK_SET);

}


void fprintRestOfLine(FILE *readPtr, FILE *writePtr)
{
	int c;

	while ((c=fgetc(readPtr))!=EOF && c!='\n')
			fputc(c, writePtr);
	if (c=='\n')
		fputc(c, writePtr);

}

Macro *newMacro()
{
	Macro *ptr = NULL;

	if(!(ptr = (Macro *) malloc(sizeof(Macro))))
		return NULL;

	ptr->startPos = -1;
	ptr->endPos = -1;
	return ptr;
}

void deleteMacro(Macro *macro)
{
	free(macro);
	macro = NULL;
}
