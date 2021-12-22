#include <libraries.h>
#include <types.h>
#include <macro.h>
#include <hashTable.h>

int main(int argc, char **argv)
{
	if (argc == 1) {
		fprintf(stderr, "Error!\nPlease provide the names of the files when starting the program");
		return EXIT_FAILURE;
	}
	while (argc>1) {
		FILE *fp = NULL;
		MacroTable macroTable;
		fp = fopen(argv[argc-1], "r+");
		if (!fp) {
			fprintf(stderr, "Error!\nInvalid file name: %s", argv[argc-1]);
			return EXIT_FAILURE;
		}
		initTable(&macroTable, TABLE_SIZE);
		fscanAndExpandMacros(fp, &macroTable, 0);
		deleteTable(&macroTable);
		fclose(fp);
		argc--;
	}
	return EXIT_SUCCESS;
}