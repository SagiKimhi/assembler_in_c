/* Todo Note:
 * We may want to create an additional function called save macro
 * and replace it with the long macro saving process inside the expandMacros function
*/
#include "macro.h"
#include <preprocessor.h>

static int fscanAndExpandMacros(FILE *readPtr, FILE *writePtr, Tree *binTree);

/* Also temporary until a data structure is decided for the preprocessor */
void macroPreprocessor(FILE *read, FILE *write)
{
	Tree *binTree = newTree();

	if (!read || !write)
		return;

	fscanAndExpandMacros(read, write, binTree);
	deleteTree(binTree, deleteMacro);
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

			/* Create a new macro and save it into the data structure */
			macro = newMacro();
			skipSpaces(readPtr);
			setStartPosition(macro, ftell(readPtr));
			addTreeNode(binTree, tempWord, macro);

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

		macro = getData(searchTreeNode(binTree, tempWord));

		if (macro!=NULL)
			fprintMacro(readPtr, writePtr, macro);
		else
			putStreamLine(readPtr, tempFilePosition, writePtr, ftell(writePtr));
	}

	return 0;
}
