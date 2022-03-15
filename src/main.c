#include <errors.h>
#include <libraries.h>
#include <assembler.h>
#include <preprocessor.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	/* Variable Definitions */
	int arg;

	/* Ensure that command line args were provided */
	if (argc == 1) {
		__ERROR__MISSING_ARGS(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Begin assembling the files */
	for (arg = 1; arg < argc; arg++) {
		/* Start the preprocessor */
		if (macroPreprocessor(argv[arg])==EXIT_SUCCESS)
			/* Start the assembler */
			startAssembler(argv[arg]);
	}

	return 0;
}

