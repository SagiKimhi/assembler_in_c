#include "labels.h"
#include <errors.h>
#include <libraries.h>
#include <firstPass.h>
#include <binaryTree.h>
#include <preprocessor.h>

int main(int argc, char **argv)
{
	/* Variable Definitions */
	int arg;
	FILE *wFilePtr, *rFilePtr;
	char filename[FILENAME_MAX+1] = {0};

	/* Data structure definitions */
	Tree *symbolTree = newTree();

	/* Ensure that command line args were provided */
	if (argc == 1) {
		__ERROR__MISSING_ARGS(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Begin assembling the files */
	for (arg = 1; arg < argc; arg++) {
		/* Make sure the filename's length is valid */
		if (strlen(argv[arg])>(FILENAME_MAX-MAX_FILE_EXTENSION_LEN)) {
			__ERROR__FILE_NAME_TOO_LONG(argv[arg])
			continue;
		}

		strcpy(filename, argv[arg]);				/* Copy the file name */
		strcat(filename, ".as");					/* Add ".as" file extension */

		/* Attempt to open file for reading */
		if (!(rFilePtr = fopen(filename, "r"))) {
			__ERROR__INVALID_FILE_NAME(filename)
			continue;
		}

		filename[strlen(filename)-1] = 'm';			/* Change file extension to ".am" */

		/* Attempt to open file for writing */
		if (!(wFilePtr = fopen(filename, "w"))) {
			fclose(rFilePtr);
			__ERROR__UNABLE_TO_CREATE_FILE
			continue;
		}

		/* Start the preprocessor */
		macroPreprocessor(rFilePtr, wFilePtr);

		fclose(rFilePtr);
		fclose(wFilePtr);

		if (!(rFilePtr = fopen(filename, "r"))) {
			__ERROR__INVALID_FILE_NAME(filename)
			continue;
		}

		if (startFirstPass(rFilePtr, symbolTree))
			printTree(stdout, symbolTree, printLabel);

		deleteTree(symbolTree, deleteLabel);
		fclose(rFilePtr);
	}

	return 0;
}

