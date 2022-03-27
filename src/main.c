/* This is an assembler program.
 * This program recieves paths to the files to be assembled from
 * the command line, and assembles them.
 * The specified file paths should exclude the file's '.as' extension
 * and should only provide the mention the path to the file as well as its basename.
 *
 * Each successfuly assembled file should result with the following output files:
 *
 * filename.am - The post processed file, after removal of empty lines and expansion of
 * all macro calls of the source file's defined macros.
 *
 * filename.ob - the assembled object file.
 * 
 * filename.ent - a file which contains the base and offset addresses of all entries declared
 * in the source file.
 *
 * filename.ext - a file which contains all the memory addresses which should be replaced
 * with the base and offset addresses of labels which were used in this source file as operands
 * and are declared as extern.
 *
 * upon success in assembling all of the files from the command line, EXIT_SUCCESS will be returned.
 * 
 * if any assembly operation failed, then EXIT_FAILURE will be returned.
 *
 * @Authors Sagi Kimhi, Sagi Efrati
 */

#include <errors.h>
#include <libraries.h>
#include <assembler.h>
#include <preprocessor.h>

int main(int argc, char **argv)
{
	/* Variable Definitions */
	int arg;
	int validFlag = EXIT_SUCCESS;

	/* Ensure that command line args were provided */
	if (argc == 1) {
		__ERROR__MISSING_ARGS(argv[0]);
		return EXIT_FAILURE;
	}

	/* Begin assembling the files */
	for (arg = 1; arg < argc; arg++) {
		if (macroPreprocessor(argv[arg])==EXIT_SUCCESS) {
			if (startAssembler(argv[arg])==EXIT_FAILURE)
				validFlag = EXIT_FAILURE;
		}
		else
			validFlag = EXIT_FAILURE;
	}

	return validFlag;
}

