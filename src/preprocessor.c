/* Todo Note:
 * We may want to create an additional function called save macro
 * and replace it with the long macro saving process inside the expandMacros function
*/
#include <preprocessor.h>

static int fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, Tree *binTree);

/* Also temporary until a data structure is decided for the preprocessor */
void macroPreprocessor(FILE *read, FILE *write)
{
	Tree *binTree = newTree();

	if (!read || !write)
		return;

	fscanAndExpandMacros(read, write, binTree);
	deleteTree(binTree);
}

/* fscanAndExpandMacros: Scans macro definitions from the file pointed to by readPtr, and writes
 * the 'processed version' into the file pointed to by writePtr. 
 * 'Processed version' means that macro definitions are not included in the new file,
 * and all macro calls are replaced by the actual contents of those defined macros. */
static int fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, Tree *binTree)
{
	/* Variable Definitions */
	Macro *macro;
	int tempFilePosition;
	char tempWord[MAX_LINE_LEN+1];

	while (skipSpaces(readPtr)!=EOF) {
		/* Get the current readPtr file position */
		tempFilePosition = ftell(readPtr);

		/* Error checking */
		if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)<=0) 
			return EOF;

		/* Check for begining of macro definition */
		if (!strcmp(tempWord, START_OF_MACRO_DEFINITION)) {
			/* Get the name of the macro + Error checking */
			if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)<=0)
				return EOF;

			skipSpaces(readPtr);

			/* Create a new macro and save it into the data structure */
			macro = newMacro();
			setStartPosition(macro, ftell(readPtr));
			binTree->root = addTreeNode(binTree->root, tempWord, macro);

			/* Find the end of macro definition, and save the end of macro 
			 * definition's file index to the new macro structure */
			while (strcmp(tempWord, END_OF_MACRO_DEFINITION)) {
				skipSpaces(readPtr);
				tempFilePosition = ftell(readPtr);

				if (getWord(tempWord, MAX_LINE_LEN+1, readPtr)<=0)
					return EOF;
			}
			setEndPosition(macro, tempFilePosition);
			continue;
		}

		/* If the first word is a previously defined macro, print the 
		 * contents of the macro to writePtr with fprintMacro, otherwise 
		 * print the line 'as is' and start the function over from the next line */
		if ((macro = treeSearch(binTree->root, tempWord))!=NULL)
			fprintMacro(readPtr, writePtr, macro);
		else
			putStreamLine(readPtr, tempFilePosition, writePtr, ftell(writePtr));
	}

	return 0;
}
