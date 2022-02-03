/* Todo Note:
 * We may want to create an additional function called save macro
 * and replace it with the long macro saving process inside the expandMacros function
*/

#include <preprocessor.h>

/* Temporary static functions until we'll decide which data struct we're gonna use */
static void macroPreprocessor_tree(FILE *read, FILE *write);
static void macroPreprocessor_list(FILE *read, FILE *write);
static void macroPreprocessor_table(FILE *read, FILE *write);
static int fscanAndExpandMacros_tree(FILE *readPtr, FILE *writePtr, Tree *binTree, int macroFlag);
static int fscanAndExpandMacros_list(FILE *readPtr, FILE *writePtr, WordList *wordList, int macroFlag);
static int fscanAndExpandMacros_table(FILE *readPtr, FILE *writePtr, HashTable *hashTable, int macroFlag);

/* Also temporary until a data structure is decided for the preprocessor */
void macroPreprocessor(FILE *read, FILE *write)
{
	if (USE_LIST)
		macroPreprocessor_list(read, write);
	else if (USE_TABLE)
		macroPreprocessor_table(read, write);
	else
		macroPreprocessor_tree(read, write);
}

/* Uses a Binary Tree data structure for the preprocessing operation (This is the best
 * fit for this purpose in my opinion, both timewise, and also since the binary tree's source code
 * is super clean compared to the other data structures). */
static void macroPreprocessor_tree(FILE *read, FILE *write)
{
	Tree *binTree = newTree();

	if (!read || !write)
		return;

	fscanAndExpandMacros_tree(read, write, binTree, 0);
	deleteTree(binTree);
}

/* fscanAndExpandMacros: Scans macro definitions from the file pointed to by readPtr, and writes
 * the 'processed version' into the file pointed to by writePtr. 
 * 'Processed version' means that macro definitions are not included in the new file,
 * and all macro calls are replaced by the actual contents of those defined macros. 
 */
static int fscanAndExpandMacros_tree(FILE *readPtr, FILE *writePtr, Tree *binTree, int macroFlag)
{
	Macro *macro;
	char word[MAX_LINE_LEN];
	char line[MAX_LINE_LEN];
	int bytesScanned=0,tempPos = ftell(readPtr);

	/* Get the next line from the file stream */
	if (!fgets(line, MAX_LINE_LEN, readPtr)) 
		return -1;

	/* Save the first word in the line, and count the bytes up to the word following it,
	 * if no word was scanned - print the empty line and begin another iteration */
	if (sscanf(line, " %s %n", word, &bytesScanned) != 1) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, macroFlag);
	}

	/* check if the first word indicates the begining of a macro definition */
	if (!macroFlag && !strcmp(word, START_OF_MACRO_DEFINITION)) {
		int temp = 0;
		/* Check if there's also a macro name, and save it to 'word' */
		if (sscanf(&line[bytesScanned], " %s %n", word, &temp) == 1) {
			bytesScanned += temp;
			macroFlag = 1;
		}
	}

	/* saving the macro to the data structure 
	 * (if we're indeed inside a macro definition) */
	if (macroFlag) {
		char *tempWord;
		macro = newMacro();
		/* save the file's position of the macro definition */
		macro->startPos = tempPos + bytesScanned; 

		/* if the end of macro defiinition is not in this line, keep advancing
		 * the file position indicator until reaching end of function definition */
		if (!(tempWord=strstr(line, END_OF_MACRO_DEFINITION)))
			while ((fscanf(readPtr, " %s ", line)==1) && strcmp(line, END_OF_MACRO_DEFINITION))
				tempPos=ftell(readPtr);
		else
			tempPos += (tempWord-line);

		macro->endPos = tempPos;
		binTree->root = addTreeNode(binTree->root, word, macro);

		return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, 0);
	}

	/* If the first word is a previously defined macro, print the 
	 * contents of the macro to writePtr with fprintMacro, otherwise 
	 * print the line 'as is' and start the function over from the next line */
	if ((macro = treeSearch(binTree->root, word))!=NULL)
		fprintMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

	return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, 0);
}


/* Uses a list data structure for the preprocessing operation */
static void macroPreprocessor_list(FILE *read, FILE *write)
{
	WordList *wordList = newWordList();

	if (!read || !write)
		return;

	if (!wordList) {
		fprintf(stderr, "Error: unable to initialize list. Aborting...\n");
		exit(EXIT_FAILURE);
	}

	fscanAndExpandMacros_list(read, write, wordList, 0);
	w_deleteList(wordList);
}

/* Uses a HashTable data structure for the preprocessing operation */
static void macroPreprocessor_table(FILE *read, FILE *write)
{
	HashTable *hashTable = newHashTable(TABLE_SIZE);

	if (!read || !write)
		return;

	if (!hashTable) {
		fprintf(stderr, "Error: unable to initialize table. Aborting...\n");
		exit(EXIT_FAILURE);
	}

	fscanAndExpandMacros_table(read, write, hashTable, 0);
	deleteTable(hashTable, &deleteMacro);
}


static int fscanAndExpandMacros_table(FILE *readPtr, FILE *writePtr, HashTable *hashTable, int macroFlag)
{
	Macro *macro;
	char word[MAX_LINE_LEN];
	char line[MAX_LINE_LEN];
	int bytesScanned=0,tempPos = ftell(readPtr);

	if (!fgets(line, MAX_LINE_LEN, readPtr)) 
		return -1;

	if (sscanf(line, " %s %n", word, &bytesScanned) != 1) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros_table(readPtr, writePtr, hashTable, macroFlag);
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

		if (!(tempWord=strstr(line, "endm")))
			while ((fscanf(readPtr, " %s ", line)==1) && strcmp(line, "endm"))
				tempPos=ftell(readPtr);
		else
			tempPos += (tempWord-word);

		macro->endPos = tempPos;

		if (!insert(hashTable, word, macro)) {
			fprintf(stderr, "Error: macro inserion failed. macro name: %s\n", word);
			exit(EXIT_FAILURE);
		}

		return fscanAndExpandMacros_table(readPtr, writePtr, hashTable, 0);
	}

	if ((macro = (Macro *)search(hashTable, word))!=NULL)
		fprintMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

	return fscanAndExpandMacros_table(readPtr, writePtr, hashTable, 0);
}

static int fscanAndExpandMacros_list(FILE *readPtr, FILE *writePtr, WordList *wordList, int macroFlag)
{
	Macro *macro;
	char word[MAX_LINE_LEN];
	char line[MAX_LINE_LEN];
	int bytesScanned=0,tempPos = ftell(readPtr);

	if (!fgets(line, MAX_LINE_LEN, readPtr)) 
		return -1;

	if (sscanf(line, " %s %n", word, &bytesScanned) != 1) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros_list(readPtr, writePtr, wordList, macroFlag);
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

		if (!(tempWord=strstr(line, "endm")))
			while ((fscanf(readPtr, " %s ", line)==1) && strcmp(line, "endm"))
				tempPos=ftell(readPtr);
		else
			tempPos += (tempWord-word);

		macro->endPos = tempPos;

		if (w_searchList(wordList, word) != NULL) {
			fprintf(stderr, "Error: macro \"%s\" already defined. Aborting...", word);
			exit(EXIT_FAILURE);
		}

		if (!w_addToEnd(wordList, word, macro)) {
			fprintf(stderr, "Error: macro inserion failed. macro name: %s\n", word);
			exit(EXIT_FAILURE);
		}

		return fscanAndExpandMacros_list(readPtr, writePtr, wordList, 0);
	}

	if ((macro = w_searchList(wordList, word))!=NULL)
		fprintMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

	return fscanAndExpandMacros_list(readPtr, writePtr, wordList, 0);
}

