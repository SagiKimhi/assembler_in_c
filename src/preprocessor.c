#include <preprocessor.h>

static void MacroPreproccessor_tree(FILE *read, FILE *write);
static void MacroPreproccessor_list(FILE *read, FILE *write);
static void MacroPreproccessor_table(FILE *read, FILE *write);
static int fscanAndExpandMacros_tree(FILE *readPtr, FILE *writePtr, Tree *binTree, int macroFlag);
static int fscanAndExpandMacros_list(FILE *readPtr, FILE *writePtr, WordList *wordList, int macroFlag);
static int fscanAndExpandMacros_table(FILE *readPtr, FILE *writePtr, HashTable *hashTable, int macroFlag);

void MacroPreproccessor(FILE *read, FILE *write)
{
	if (USE_LIST)
		MacroPreproccessor_list(read, write);
	else if (USE_TABLE)
		MacroPreproccessor_table(read, write);
	else if (USE_TREE)
		MacroPreproccessor_tree(read, write);
}

static void MacroPreproccessor_list(FILE *read, FILE *write)
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

static void MacroPreproccessor_table(FILE *read, FILE *write)
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

static void MacroPreproccessor_tree(FILE *read, FILE *write)
{
	Tree *binTree = newTree();

	if (!read || !write)
		return;

	fscanAndExpandMacros_tree(read, write, binTree, 0);
	deleteTree(binTree);
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

	if ((macro = (Macro *) w_searchList(wordList, word))!=NULL)
		fprintMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

	return fscanAndExpandMacros_list(readPtr, writePtr, wordList, 0);
}

static int fscanAndExpandMacros_tree(FILE *readPtr, FILE *writePtr, Tree *binTree, int macroFlag)
{
	Macro *macro;
	char word[MAX_LINE_LEN];
	char line[MAX_LINE_LEN];
	int bytesScanned=0,tempPos = ftell(readPtr);

	if (!fgets(line, MAX_LINE_LEN, readPtr)) 
		return -1;

	if (sscanf(line, " %s %n", word, &bytesScanned) != 1) {
		fprintf(writePtr, "%s", line);
		return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, macroFlag);
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

		if (!(binTree->root = addTreeNode(binTree->root, word, macro))) {
			fprintf(stderr, "Error: macro inserion failed. macro name: %s\n", word);
			exit(EXIT_FAILURE);
		}

		return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, 0);
	}

	if ((macro = (Macro *)treeSearch(binTree->root, word))!=NULL)
		fprintMacro(readPtr, writePtr, macro);
	else
		fprintf(writePtr, "%s", line);

	return fscanAndExpandMacros_tree(readPtr, writePtr, binTree, 0);
}
