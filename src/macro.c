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
	HashTable *hashTable = NULL;

	if (!read || !write)
		return;
	if (!(initTable(hashTable, TABLE_SIZE)))
		return;

	fscanAndExpandMacros(read, write, hashTable, 0);
	deleteTable(hashTable);

}

int fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, HashTable *hashTable, int macroFlag)
{
	Macro *macro;
	char word[MAX_LINE_LEN];
	char line[MAX_LINE_LEN];
	int bytesScanned=0,tempPos = ftell(readPtr);

	if (!fgets(line, MAX_LINE_LEN, readPtr)) 
		return -1;

	if (sscanf(line, " %s %n", word, &bytesScanned) != 2) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros(readPtr, writePtr, hashTable, macroFlag);
	}

	if (!macroFlag && !strcmp(word, "macro")) {
		int temp = 0;
		if (sscanf(line, " %s %n", word, &temp)==2) {
			tempPos += (temp+bytesScanned);
			macroFlag = 1;
		}
	}
	if (macroFlag) {
		char *tempWord;

		macro = newMacro();
		macro->startPos = tempPos;

		if (!(tempWord=strstr(line, "endm")))
			while ((fscanf(readPtr, " %s ", tempWord)==1) && strcmp(tempWord, "endm")) {
				if(feof(readPtr)) {
					fprintf(stderr, "Error: reached eof before end of macro definition");
					return -1;
				}
				tempPos=ftell(readPtr);
			}
		else
			tempPos += (tempWord-word);

		macro->endPos = tempPos;

		if (!insert(hashTable, macro, word)) {
			fprintf(stderr, "Error: macro inserion failed. macro name: %s\n", word);
			exit(EXIT_FAILURE);
		}

		return fscanAndExpandMacros(readPtr, writePtr, hashTable, 0);
	}

	if ((macro = (Macro *) search(hashTable, word))!=NULL)
		fexpandMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

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
}

