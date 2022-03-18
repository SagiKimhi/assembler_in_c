#include <preprocessor.h>

/* ----------------------------------------------------------------	*
 *						Static Function Prototypes					*
 * ----------------------------------------------------------------	*/
/* saveMacro: defines a new macro and saves it in the macro tree. */
static void saveMacro(FILE *readPtr, Tree *macroTree);

/* fscanAndExpandMacros: scans the file pointed to by readPtr, and copies it's content
 * onto writePtr while leaving out macro definitions found in readPtr, and replacing
 * macro calls found in readPtr by the actual contents of the defined macro. */
static void fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, Tree *binTree);
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Public Functions						*
 * ----------------------------------------------------------------	*/
/* macroPreprocessor: opens the provided filename with a '.as' extension for reading,
 * and creates a new file with an identical filename and a '.am' extension for writing.
 * The newly created file will contain the processed contents of the original file, 
 * leaving macro definitions out of the new file and replacing macro calls with 
 * the actual contents of the defined macros.
 * Returns EXIT_SUCCESS upon success or EXIT_FAILURE if an error occured.*/
int macroPreprocessor(const char *fileName)
{
	Tree *macroTree;
	FILE *readPtr, *writePtr;

	readPtr=openFile(fileName, SOURCE_FILE_EXTENSION, "r");

	if (!readPtr)
		return EXIT_FAILURE;

	writePtr=openFile(fileName, PREPROCESSED_FILE_EXTENSION, "w");

	if (!writePtr) {
		fclose(readPtr);
		return EXIT_FAILURE;
	}

	if (!(macroTree = newTree())) {
		fclose(readPtr);
		fclose(writePtr);
		deleteFile(fileName, PREPROCESSED_FILE_EXTENSION);
		return EXIT_FAILURE;
	}

	fscanAndExpandMacros(readPtr, writePtr, macroTree);
	deleteTree(macroTree, deleteMacro);
	fclose(readPtr);
	fclose(writePtr);

	return EXIT_SUCCESS;
}
/* ----------------------------------------------------------------	*/

/* ----------------------------------------------------------------	*
 *							Static Functions						*
 * ----------------------------------------------------------------	*/
/* fscanAndExpandMacros: scans the file pointed to by readPtr, and copies it's content
 * onto writePtr while leaving out macro definitions found in readPtr, and replacing
 * macro calls found in readPtr by the actual contents of the defined macro. */
static void fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, Tree *macroTree)
{
	Macro *macro;
	int tempFilePosition;
	char tempWord[MAX_LINE_LEN+1];

	while (skipSpaces(readPtr)!=EOF) {
		/* Get the current readPtr file position */
		tempFilePosition = ftell(readPtr);

		/* Error checking */
		if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)<=0) 
			return;

		/* Check for begining of macro definition */
		if (!strcmp(tempWord, START_OF_MACRO_DEFINITION)) {
			saveMacro(readPtr, macroTree);
			continue;
		}

		/* check if the current word is the name of a macro */
		macro = getTreeNodeData(searchTreeNode(macroTree, tempWord));

		if (macro!=NULL)
			fprintMacro(readPtr, writePtr, macro);
		else
			putStreamLine(readPtr, tempFilePosition, writePtr, ftell(writePtr));
	}
}

/* saveMacro: defines a new macro and saves it in the macro tree. */
static void saveMacro(FILE *readPtr, Tree *macroTree)
{
	Macro *macro = NULL;
	int tempFilePosition;
	char tempWord[MAX_LINE_LEN+1];

	/* Get the name of the macro + Error checking */
	if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)==EOF)
		return;

	/* If we reached the end of macro definition before a macro name was 
	 * even provided, we simply ignore the definition and move on. */
	if (!strcmp(tempWord, END_OF_MACRO_DEFINITION))
		return;

	/* Check if a macro with this name already exists in the tree */
	macro = getTreeNodeData(searchTreeNode(macroTree, tempWord));

	if (!macro) {
		/* Create a new macro and save it into the data structure */
		macro = newMacro();
		addTreeNode(macroTree, tempWord, macro);
	}
	else {
		/* TODO: print warning, this macro was already defined previously, 
		 * overwriting previous macro definition with current definition. */
	}

	/* skip to the starting point of the macro's actual content
	 * and set the apropriate file position. */
	skipSpaces(readPtr);
	setStartPosition(macro, ftell(readPtr));

	/* seek to the end of macro definition */
	while (strcmp(tempWord, END_OF_MACRO_DEFINITION)) {
		skipSpaces(readPtr);
		tempFilePosition = ftell(readPtr);

		if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)==EOF)
			break;
	}

	setEndPosition(macro, tempFilePosition);
}
/* ----------------------------------------------------------------	*/
