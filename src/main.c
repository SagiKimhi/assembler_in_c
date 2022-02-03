#include <errors.h>
#include <libraries.h>
#include <preprocessor.h>

int main(int argc, char **argv)
{
	/* Variable initialization */
	char filename[FILENAME_MAX] = {0};
	FILE *wFilePtr, *rFilePtr;
	int arg;

	if (argc == 1)
		EXIT_ERROR(MISSING_ARGUMENTS, "");

	/* Preprocessor Stage */
	for (arg = 1; arg < argc; arg++) {
		strcpy(filename, argv[arg]);
		strcat(filename, ".as"); /* Add the .as prefix to the filename */

		if (!(rFilePtr = fopen(filename, "r")))
			EXIT_ERROR(INVALID_FILE_NAME, filename);

		filename[strlen(filename)-1] = 'm'; /* replaces the ".as" file postifix to ".am" */

		if (!(wFilePtr = fopen(filename, "w")))
			EXIT_ERROR(UNABLE_TO_OPEN_FILE, "");

		macroPreprocessor(rFilePtr, wFilePtr);
		fclose(rFilePtr);
		fclose(wFilePtr);
	}

	

	return 0;
}

