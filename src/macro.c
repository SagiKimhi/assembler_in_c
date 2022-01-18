#include <macro.h>

void MacroPreproccessor(FILE *read, FILE *write)
{
	HashTable *hashTable = newHashTable();

	if (!read || !write)
		return;

	if (!hashTable) {
		fprintf(stderr, "Error: unable to initialize table\n");
		return;
	}

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

	if (sscanf(line, " %s %n", word, &bytesScanned) != 1) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros(readPtr, writePtr, hashTable, macroFlag);
	}

	if (!macroFlag && !strcmp(word, "macro")) {
		int temp = 0;
		if (sscanf(&line[bytesScanned], " %s %n", word, &temp) == 1) {
			tempPos += (temp+bytesScanned);
			macroFlag = 1;
		}
	}

	if (macroFlag) {
		char *tempWord;
		macro = newMacro();
		macro->startPos = tempPos;

		if (!(tempWord=strstr(line, "endm"))) {
			while ((fscanf(readPtr, " %s ", line)==1) && strcmp(line, "endm")) {
				if(feof(readPtr)) {
					fprintf(stderr, "Error: reached eof before end of macro definition");
					return -1;
				}
				tempPos=ftell(readPtr);
			}
		}
		else
			tempPos += (tempWord-word);

		macro->endPos = tempPos;

		if (!insert(hashTable, macro, word)) {
			fprintf(stderr, "Error: macro inserion failed. macro name: %s\n", word);
			exit(EXIT_FAILURE);
		}
		else
			fprintf(stderr, "inserted macro: %s\nstartPos: %d, endPos: %d\n", word, macro->startPos, macro->endPos);

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

