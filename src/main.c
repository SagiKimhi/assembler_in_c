#include <errors.h>
#include <libraries.h>
#include <preprocessor.h>

int main(int argc, char **argv)
{
	/* Variable initialization */
	int arg;
	FILE *wFilePtr, *rFilePtr;
	char filename[FILENAME_MAX] = {0};

	if (argc == 1)
		__EXIT_ERROR__MISSING_ARGS(argv[0])

	/* Preprocessor Stage */
	for (arg = 1; arg < argc; arg++) {
		strcpy(filename, argv[arg]);
		strcat(filename, ".as");					/* Add ".as" postfix to the filename */

		if (!(rFilePtr = fopen(filename, "r")))		/* Attempt to open file for reading */
			__ERROR__INVALID_FILE_NAME(filename)

		filename[strlen(filename)-1] = 'm';			/* Change file postfix to ".am" */

		if (!(wFilePtr = fopen(filename, "w"))) {	/* Attempt to open file for writing */
			fclose(rFilePtr);
			__ERROR__UNABLE_TO_CREATE_FILE
		}

		macroPreprocessor(rFilePtr, wFilePtr);		/* Initiate preprocessing operation */

		fclose(rFilePtr);
		fclose(wFilePtr);
	}

	return 0;
}

